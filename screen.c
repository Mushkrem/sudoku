#pragma once

#include "screen.h"
#include "utils.h"

#define CLEAR "\x1B[2J\x1B[H"
#define DEFAULT_BOUNDS (_bounds){0, 0}

_menu* get_menu_instance(_bounds bounds);
_bounds bounds;

void s_init() {
	printf("\33[?25l"); // Hide cursor

	// Disable legacy console mode (so the ansi escape characters work)
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw = 0;
	GetConsoleMode(h, &dw);
	dw |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(h, dw);
	//

	bounds.width = Utils.GetConsoleWidth();
	bounds.height = Utils.GetConsoleHeight();
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