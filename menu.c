#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "utils.h"
#include "interface.h"

static _menu* instance = NULL;

_menu* get_menu_instance(void);

int m_select_impl(int n) {
	_menu* menu = get_menu_instance();
	int p = menu->position;
	if (p == n)
		return EXIT_FAILURE;

	menu->position = n;
	menu->update();
	return EXIT_SUCCESS;
}

int m_update_impl() {
	_menu* menu = get_menu_instance();
	Interface.draw_text((COORD) { 2, 2 }, L"The selected value is: %d", instance->position);
	return EXIT_SUCCESS;
}

_menu* get_menu_instance() {
	if (instance != NULL)
		return instance;

	instance = (_menu*)malloc(sizeof(_menu));
	if (instance == NULL)
		exit(EXIT_FAILURE);

	instance->position = 0;
	instance->labels = NULL;
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	return instance;
}