#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "utils.h"
#include "interface.h"

#define Y_OFFSET 4

static _menu* instance = NULL;

_menu* get_menu_instance(int w);

int width;
const wchar_t* buttons[16];
int button_count = 0;

void m_draw_buttons() {
	for (int i = 0; buttons[i] != NULL; i++) {
		wchar_t* higher_intensity = L"\033[1m";
		if (instance->position != i + 1) // sets the text to a higher intensity if its the selected one
			higher_intensity = L"";
		Interface.draw_text((COORD) { width / 2 - wcslen(buttons[i]) / 2, 6 + i * 2 }, L"%s%s", higher_intensity, buttons[i]);
		button_count = i + 1;
	}
}

void m_init() {
	instance->position = 2;
	instance->previous_position = NULL;
	instance->labels = NULL;

	buttons[0] = L"Play";
	buttons[1] = L"Options";
	buttons[2] = L"Exit";

	m_draw_buttons();

	Interface.draw_text((COORD) { width/2 - 6, Y_OFFSET - 2 }, L"\033[7m\033[94m S U D O K U \033[0m");
}

int m_select_impl(int n) {
	_menu* menu = get_menu_instance(NULL);
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

int m_update_impl() {
	_menu* menu = get_menu_instance(NULL);
	int p = instance->previous_position;
	int n = instance->position;

	// should make an external function to draw the cursor (changing its visuals depending on where in the menu tree/game you are)
	Interface.draw_text((COORD) { width / 2 + wcslen(buttons[p-1]), 4 + (p) * 2 }, L" ");
	Interface.draw_text((COORD) { width / 2 + wcslen(buttons[n-1]), 4 + (n) * 2 }, L"\033[6m\033[94m◀");

	m_draw_buttons();

	return EXIT_SUCCESS;
}

_menu* get_menu_instance(int w) {
	if (instance != NULL)
		return instance;

	instance = (_menu*)malloc(sizeof(_menu));
	if (instance == NULL)
		exit(EXIT_FAILURE);

	if (w != NULL) {
		width = w;
	}

	m_init();
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	return instance;
}