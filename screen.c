#pragma once

#include "utils.h"
#include "screen.h"
#include "interface.h"

#define CLEAR "\x1B[2J\x1B[H"
#define DEFAULT_BOUNDS (_bounds){0, 0}

static _screen* instance = NULL;

_menu* get_menu_instance(_bounds bounds);
_screen* get_screen_instance(_game* game_ref);

_bounds bounds;
_game* game_ref;
int _is_legacy = NULL;
int _theme = 4;

_game* get_game_ref() {
	return game_ref;
}

int get_theme() {
	return _theme;
}

void set_theme(int n) {
	if (n > 7) n = 0;
	if (n < 0) n = 7;
	_theme = n;
}

int is_legacy_console() {
	if (_is_legacy != NULL) return _is_legacy;

	if (Utils.get_character_at((COORD) { 0, 0 }) != 32) { // At 0, 0 is the ansi code for hiding the cursor, which is later replaced with '32' in modern terminals
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

	bounds.width = Utils.get_console_width();
	bounds.height = Utils.get_console_height();
}

int update_impl() {
	_menu* menu = get_menu_instance(bounds);
	while (TRUE) {
		if (bounds.width != Utils.get_console_width() || bounds.height != Utils.get_console_height()) {
			system("cls"); // should handle it in some other way
			menu->update();
			if (game_ref->started)
				game_ref->update();
		}
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

_screen* get_screen_instance(_game* ref) {
	if (instance != NULL)
		return instance;

	if (ref != NULL) {
		game_ref = ref;
	}

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
	instance->get_game_ref = get_game_ref;
	instance->menu = get_menu_instance(DEFAULT_BOUNDS);
	return instance;
}