#pragma once

#include "screen.h"

#define CLEAR "\x1B[2J\x1B[H"

_menu* get_menu_instance(void);

int go_to(COORD c) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, (COORD){ c.X, c.Y });
	return EXIT_SUCCESS;
}

int update_impl() {
	_menu* menu = get_menu_instance();
	while (TRUE) {
		system("cls");
		go_to((COORD) { 0, 0 });
	}
	return EXIT_SUCCESS;
}

int select_impl(int n) {
	_menu* menu = get_menu_instance();
	menu->select(n);

	return EXIT_SUCCESS;
}

SCREEN get_screen() {
	SCREEN screen;
	screen.update = update_impl;
	screen.select = select_impl;

	screen.menu = get_menu_instance();
	
	return screen;
}