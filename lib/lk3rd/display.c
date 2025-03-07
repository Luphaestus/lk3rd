/*
 * Copyright (c) 2024 Igor Belwon <igor.belwon@mentallysanemainliners.org>
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 */
#include <lib/font_display.h>
#include <lib/version.h>
#include <stdlib.h>
#include <string.h>

#include "include/lk3rd/display.h"
#include "include/lk3rd/fastboot_menu.h"
#include "include/lk3rd/mainline_quirks.h"

void draw_line_lcd(int color_fg, int color_bg)
{
	char *str = malloc(MAX_NUM_CHAR_PER_LINE + 1);
	memset(str, '-', MAX_NUM_CHAR_PER_LINE);
	str[MAX_NUM_CHAR_PER_LINE] = '\0';
	print_lcd_update(color_fg, color_bg, str);
}

void draw_action(u32 font_color, const char *fmt, ...)
{
	draw_line_lcd(font_color, FONT_BLACK);
	print_lcd_update(font_color, FONT_BLACK, fmt);
	draw_line_lcd(font_color, FONT_BLACK);
}

const char* get_action_text(enum action current_action)
{
	switch(current_action)
	{
		case ACTION_START:
			return "START";
		case ACTION_REBOOT_RECOVERY:
			return "Reboot recovery";
		case ACTION_REBOOT_BOOTLOADER:
			return "Reboot bootloader";
		case ACTION_REBOOT_FASTBOOTD:
			return "Reboot FastbootD";
		case ACTION_REBOOT_DOWNLOAD:
			return "Reboot Download";
		case ACTION_POWEROFF:
			return "Power off";
		default:
			return "Unknown action";
	}
}

u32 get_action_colour(enum action current_action)
{
	switch(current_action)
	{
		case ACTION_START:
			return FONT_GREEN;
		case ACTION_REBOOT_RECOVERY:
			return FONT_YELLOW;
		case ACTION_REBOOT_BOOTLOADER:
			return FONT_RED;
		case ACTION_REBOOT_FASTBOOTD:
			return FONT_ORANGE;
		case ACTION_REBOOT_DOWNLOAD:
			return FONT_BLUE;
		case ACTION_POWEROFF:
			return FONT_RED;
		default:
			return FONT_WHITE;
	}
}

void draw_menu(enum action current_action)
{

#define INLINE_MODE
#ifdef INLINE_MODE
	clear_line(FONT_BLACK, 1); // Only clear lines that change to avoid flickering
	draw_action(get_action_colour(current_action), get_action_text(current_action));
#else 
	clear_line(FONT_BLACK, 0);
	for (int i = ACTION_START; i < ACTION_END; i++)
	{
		u32 colour = i == (int)current_action ? get_action_colour(i) : FONT_WHITE;
		draw_action(colour, get_action_text(i));
	}
#endif

	print_lcd_update(FONT_WHITE, FONT_BLACK, "");
	print_lcd_update(FONT_WHITE, FONT_BLACK, "Press volume key to select, and press power key to select");
	print_lcd_update(FONT_WHITE, FONT_BLACK, "");
	print_lcd_update(FONT_RED,   FONT_BLACK, "lk3rd FastBoot Mode", MAX_NUM_CHAR_PER_LINE);
	print_lcd_update(FONT_WHITE, FONT_BLACK, "PRODUCT_NAME - %s", version.platform);
	print_lcd_update(FONT_WHITE, FONT_BLACK, "BOOTLOADER VERSION - 2.0 (%s)", version.buildid);
	print_lcd_update(FONT_RED,   FONT_BLACK, "DEVICE STATE - unlocked");

	if(lk3rd_get_mainline_quirks() == 1)
		print_lcd_update(FONT_YELLOW, FONT_BLACK, "MAINLINE QUIRKS - enabled "); //extra space to overwrite the disabled message
	else
		print_lcd_update(FONT_GREEN, FONT_BLACK,  "MAINLINE QUIRKS - disabled");
}
