#pragma once

#include "screen.h"

#define CLEAR "\x1B[2J\x1B[H"

static _menu* instance = NULL;

_menu* get_menu_instance() {
	if (instance != NULL)
		return instance;

	instance = (_menu*)malloc(sizeof(_menu));
	if (instance == NULL) {
		printf("Error allocating memory for menu instance.\n");
		exit(EXIT_FAILURE);
	}
	instance->position = 0;
	instance->labels = NULL;
	return instance;
}

int go_to(COORD c) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, (COORD){ c.X, c.Y });
}

int select_impl(int n) {
	_menu* menu = get_menu_instance();
	menu->position = n;

	return EXIT_SUCCESS;
}

int update_impl() {
	_menu* menu = get_menu_instance();
	while (TRUE) {
		system("cls");
		//printf(CLEAR); //\x1B[41m B \x1B[42m C \x1B[43m D \x1B[0m
		go_to((COORD) { 0, 0 });
		printf("Position: \x1B[40m %d", menu->position);
	}
	return EXIT_SUCCESS;
}

SCREEN get_screen() {
	SCREEN screen;
	screen.update = update_impl;
	screen.select = select_impl;
	screen.menu = get_menu_instance();

	return screen;
}
