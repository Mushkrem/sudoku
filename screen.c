#pragma once

#include "screen.h"
#include "utils.h"

#define CLEAR "\x1B[2J\x1B[H"
#define DEFAULT_BOUNDS (_bounds){0, 0}

_menu* get_menu_instance(_bounds bounds);
_bounds bounds;

void s_init() {
	bounds.width = Utils.GetConsoleWidth();
	bounds.height = Utils.GetConsoleHeight();
	printf("\33[?25l"); // Hide cursor
}

int update_impl() {
	_menu* menu = get_menu_instance(bounds);
	while (TRUE) {
		if (bounds.width != Utils.GetConsoleWidth()) {
			system("cls");
		}
		Sleep(1);
		//
	}
	return EXIT_SUCCESS;
}

int select_impl(int n) {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	menu->select(n);

	return EXIT_SUCCESS;
}

int confirm_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	menu->confirm();
	return EXIT_SUCCESS;
}

SCREEN get_screen() {
	SCREEN screen;

	s_init();

	screen.update = update_impl;
	screen.select = select_impl;
	screen.confirm = confirm_impl;

	screen.menu = get_menu_instance(DEFAULT_BOUNDS);
	
	return screen;
}