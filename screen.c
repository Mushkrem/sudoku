#pragma once

#include "screen.h"
#include "utils.h"

#define CLEAR "\x1B[2J\x1B[H"

_menu* get_menu_instance(int w);
int width;

void s_init() {
	width = Utils.GetConsoleWidth();
	printf("\33[?25l"); // Hide cursor
}

int update_impl() {
	_menu* menu = get_menu_instance(width);
	while (TRUE) {
		if (width != Utils.GetConsoleWidth()) {
			system("cls");
		}
		Sleep(1);
		//
	}
	return EXIT_SUCCESS;
}

int select_impl(int n) {
	_menu* menu = get_menu_instance(NULL);
	menu->select(n);

	return EXIT_SUCCESS;
}

int confirm_impl() {
	return EXIT_SUCCESS;
}

SCREEN get_screen() {
	SCREEN screen;

	s_init();

	screen.update = update_impl;
	screen.select = select_impl;
	screen.confirm = confirm_impl;

	screen.menu = get_menu_instance(NULL);
	
	return screen;
}