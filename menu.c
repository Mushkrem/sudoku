#include "menu.h"
#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "utils.h"
#include "interface.h"

#define Y_OFFSET 4
#define DEFAULT_BOUNDS (_bounds){0, 0}

static _menu* instance = NULL;

_menu* get_menu_instance(_bounds bounds);

_bounds bounds;
const wchar_t* buttons[16];
int button_count = 0;

void m_draw_buttons() {
	for (int i = 0; i < 3; i++) {
		wchar_t* higher_intensity = L"\033[1m";
		if (instance->position != i + 1) // sets the text to a higher intensity if its the selected one
			higher_intensity = L"";
		Interface.draw_text((COORD) { bounds.width / 2 - wcslen(buttons[i]) / 2, (bounds.height / 2 )- 2 /* should be 2/x where x is the no. of buttons but that doesnt work? */ + i * 2 }, L"%s%s", higher_intensity, buttons[i]);
	}
}

void m_init() {
	instance->position = 2;
	instance->previous_position = NULL;
	instance->labels = NULL;

	buttons[0] = L"Play";
	buttons[1] = L"Options";
	buttons[2] = L"Exit";

	button_count = 3;

	m_draw_buttons();

	Interface.draw_text((COORD) { bounds.width /2 - 6, Y_OFFSET - 2 }, L"\033[7m\033[94m S U D O K U \033[0m");

	Utils.Debug(L"Width: %d Height:%d", bounds.width, bounds.height);
	//Interface.draw_text((COORD) {bounds.width / 2 - 3, bounds.height / 2 }, L"CENTER");
}

int m_select_impl(int n) {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	int p = menu->position;
	if (p == n)
		return EXIT_FAILURE;

	if (n <= 0)
		return EXIT_FAILURE;

	if (n > button_count)
		return EXIT_FAILURE;

	menu->previous_position = p;
	menu->position = n;
	menu->update();
	return EXIT_SUCCESS;
}

int m_confirm_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	int p = menu->position;

	if (buttons[p - 1] == L"Exit") {
		exit(EXIT_SUCCESS);
	}

	return EXIT_SUCCESS;

}

int m_update_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	int p = instance->previous_position;
	int n = instance->position;

	// should make an external function to draw the cursor (changing its visuals depending on where in the menu tree/game you are)
	//(bounds.height / 2) - ceil(2 / button_count) + i * 2
	Interface.draw_text((COORD) { bounds.width / 2 + wcslen(buttons[p-1]), (bounds.height / 2) - 4 + (p) * 2 }, L" ");	// Why is this 4
	Interface.draw_text((COORD) { bounds.width / 2 + wcslen(buttons[n-1]), (bounds.height / 2) - 4 + (n) * 2 }, L"\033[6m\033[94m◀");

	m_draw_buttons();

	return EXIT_SUCCESS;
}

_menu* get_menu_instance(_bounds b) {
	if (instance != NULL)
		return instance;

	instance = (_menu*)malloc(sizeof(_menu));
	if (instance == NULL)
		exit(EXIT_FAILURE);

	if (b.width != 0) {
		bounds.width = b.width;
	}

	m_init();
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	instance->confirm = m_confirm_impl;
	return instance;
}