#pragma once

#include "screen.h"
#include "utils.h"

#include "interface.h"

#define CLEAR "\x1B[2J\x1B[H"
#define DEFAULT_BOUNDS (_bounds){0, 0}

static _screen* instance = NULL;

_menu* get_menu_instance(_bounds bounds);
_screen* get_screen_instance();

_bounds bounds;
int _is_legacy = NULL;
int _theme = 4;

int get_theme() {
	return _theme;
}

int set_theme(int n) {
	_theme = n;
}

int is_legacy_console() {
	if (_is_legacy != NULL) return _is_legacy;

	if (Utils.GetCharacterAt((COORD) { 0, 0 }) != 32) { // At 0, 0 is the ansi code for hiding the cursor, which is later replaced with '32' in modern terminals
		_is_legacy = 1;									// but for some reason is not in the legacy ones(or one, because that's how it works for me at least)
	}
	else _is_legacy = 0;

	return _is_legacy;
}

void s_init() {
	printf("\033[?25l"); // Hide the cursor → it is later used to check whether the console is legacy or not (as legacy won't recognize the character)

	// Disable legacy console mode (so the ansi escape characters work) // Well it works only partially - changing code pages doesn't seem to work either
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw = 0;
	GetConsoleMode(h, &dw);
	dw |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(h, dw);

	if (is_legacy_console()) {
		printf("\033[?25l"); // After turning ENABLE_VIRTUAL_TERMINAL_PROCESSING the character should now work
		Interface.draw_text((COORD) { 0, 0 }, L"\033[7mlegacy mode");
	}

	bounds.width = Utils.GetConsoleWidth();
	bounds.height = Utils.GetConsoleHeight();
}

void clear_console() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cell_count;
	COORD write_coords = { 0, 0 };

	if (!GetConsoleScreenBufferInfo(handle, &csbi)) return;
	cell_count = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(handle, (TCHAR)' ', cell_count, write_coords, &count)) return;

	if (!FillConsoleOutputAttribute(handle, csbi.wAttributes, cell_count, write_coords, &count)) return;

	SetConsoleCursorPosition(handle, write_coords);
}

int update_impl() {
	_menu* menu = get_menu_instance(bounds);
	while (TRUE) {
		if (bounds.width != Utils.GetConsoleWidth()) {
			system("cls");
		}
		//menu->update(); //was a futile dream
		Sleep(1);
	}
	return EXIT_SUCCESS;
}

int select_impl(int n) {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	return menu->select(n);
}

int confirm_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	return menu->confirm();
}

_screen* get_screen_instance() {
	if (instance != NULL)
		return instance;

	instance = (_screen*)malloc(sizeof(_screen));
	if (instance == NULL)
		exit(EXIT_FAILURE);

	s_init();
	instance->update = update_impl;
	instance->select = select_impl;
	instance->confirm = confirm_impl;
	instance->is_legacy = is_legacy_console;
	instance->get_theme = get_theme;
	instance->set_theme = set_theme;
	instance->menu = get_menu_instance(DEFAULT_BOUNDS);
	return instance;
}