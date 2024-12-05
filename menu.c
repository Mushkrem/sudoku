#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "utils.h"

static _menu* instance = NULL;

_menu* get_menu_instance(void);

int m_select_impl(int n) {
	_menu* menu = get_menu_instance();
	int p = menu->position;
	if (p == n)
		return EXIT_FAILURE;

	menu->position = n;
	Utils.Debug(L"New menu selected value = %d", n);
	return EXIT_SUCCESS;
}

int m_update_impl() {
	return EXIT_SUCCESS;
}

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
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	return instance;
}