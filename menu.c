
#include "menu.h"
#include "utils.h"
#include "screen.h"
#include "sudoku.h"
#include "interface.h"

#define Y_OFFSET 4
#define DEFAULT_BOUNDS (_bounds){0, 0}

#define LEFT_TRIANGLE screen->is_legacy() == 1 ? L"\u25C4" : L"\u25C0"

static _menu* instance = NULL;

_menu* get_menu_instance(_bounds bounds);
_screen* get_screen_instance(_game* game_ref);

_bounds bounds;
_game* game_ref;
const wchar_t* buttons[10];
int button_count = 0;

void m_init(int reset) {
	_screen* screen = get_screen_instance(NULL);
	if (reset) {
		instance->position = -1;
		instance->previous_position = -1;
		instance->labels = NULL;
		game_ref = screen->get_game_ref();
	}

	buttons[0] = L"Play";
	buttons[1] = L"Options";
	buttons[2] = L"Exit";

	button_count = 3;

	if (!reset) instance->position = button_count;

	wchar_t buffer[64];
	Utils.write_literal(&buffer, sizeof(buffer), L"\033[7m\033[9%dm S U D O K U ", screen->get_theme());

	Interface.draw_text((COORD) { bounds.width /2 - 6, Y_OFFSET - 2 }, buffer);
}

int m_roll_impl(int n) {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	_screen* screen = get_screen_instance(NULL);
	int direction = 0;
	int p = menu->position;

	if (n - p == 0xA) direction = 1;
	if (n - p == 0xF) direction = -1;

	if (direction == 0) return !EXIT_FAILURE;

	if (Utils.contains(buttons[p - 1], L"Theme")) screen->set_theme(screen->get_theme() + direction);
	if (Utils.contains(buttons[p - 1], L"Grid")) game_ref->set_grid(game_ref->grid + direction);
	if (Utils.contains(buttons[p - 1], L"Difficulty")) game_ref->set_difficulty(game_ref->difficulty + direction);
	
	m_update_impl();

	return EXIT_SUCCESS;
}

int m_select_impl(int n) {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	int p = menu->position;
	if (p == n)
		return EXIT_FAILURE;

	if (n <= 0)
		return EXIT_FAILURE;

	if (n - p > 1) { // Left-right implementation
		int is_roll = m_roll_impl(n);
		if (is_roll) return EXIT_SUCCESS;
	}

	if (n > button_count)
		return EXIT_FAILURE;


	while (buttons[n - 1] == L"") { // Skip empty buttons
		if (n - p > 0) {
			n++;
		}
		else n--;
	}

	menu->previous_position = p;
	menu->position = n;
	menu->update();
	return EXIT_SUCCESS;
}

int m_confirm_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	_screen* screen = get_screen_instance(NULL);
	int p = menu->position;

	system("cls");

	if (buttons[p - 1] == L"Exit") exit(EXIT_SUCCESS);

	if (buttons[p - 1] == L"Back") m_init(FALSE);

	if (buttons[p - 1] == L"Play") {
		instance->position = 1;
		
		buttons[0] = L"Continue";
		buttons[1] = L"";
		buttons[2] = L"[%s] Difficulty";
		buttons[3] = L"[%s] Grid";
		buttons[4] = L"";

		buttons[5] = L"Back";
		button_count = 6;
	}

	if (buttons[p - 1] == L"Options") {
		instance->position = 1;

		buttons[0] = L"[4] Theme";
		buttons[1] = L"";
		buttons[2] = L"";

		buttons[3] = L"Back";
		button_count = 4;
	}

	m_update_impl();

	return EXIT_SUCCESS;
}

void m_draw_buttons() {
	for (int i = 0; i < button_count; i++) {
		wchar_t* higher_intensity = L"\033[1m";

		if (instance->position != i + 1) // sets the text to a higher intensity if its the selected one
			higher_intensity = L"";

		if (Utils.contains(buttons[i], L"]")) { // Dynamic buttons
			wchar_t temp_buffer[64] = { 0 };
			_screen* screen = get_screen_instance(NULL);

			if (Utils.contains(buttons[i], L"Theme"))
				Utils.write_literal(&temp_buffer, sizeof(temp_buffer), L"[\033[9%dm%d\033[39m] Theme", screen->get_theme(), screen->get_theme());

			if (Utils.contains(buttons[i], L"Difficulty"))
				Utils.write_literal(&temp_buffer, sizeof(temp_buffer), L"  [\033[9%dm%s\033[39m] Difficulty  ", screen->get_theme(), game_ref->get_difficulty());

			if (Utils.contains(buttons[i], L"Grid"))
				Utils.write_literal(&temp_buffer, sizeof(temp_buffer), L"[\033[9%dm%s\033[39m] Grid", screen->get_theme(), game_ref->get_grid());

			buttons[i] = _wcsdup(temp_buffer);
		}

		Interface.draw_text((COORD) { bounds.width / 2 - Utils.wcslen(buttons[i])/2, (bounds.height / 2) - 2 + i * 2 }, L"%s%s", higher_intensity, buttons[i]);
	}
}

int m_update_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	int p = instance->previous_position;
	int n = instance->position;

	_screen* screen = get_screen_instance(NULL);

	m_draw_buttons();

	if (p <= button_count) {
		Interface.draw_text((COORD) { bounds.width / 2 + Utils.wcslen(buttons[p - 1])/2 + 5, (bounds.height / 2) - 4 + (p) * 2 }, L" ");	// Why is this 4
	}

	// should make an external function to draw the cursor (changing its visuals depending on where in the menu tree/game you are)
	//(bounds.height / 2) - ceil(2 / button_count) + i * 2
	if (n <= button_count) {
		Interface.draw_text((COORD) { bounds.width / 2 + Utils.wcslen(buttons[n - 1])/2 + 5, (bounds.height / 2) - 4 + (n) * 2 }, L"\033[6m\033[9%dm%s", screen->get_theme(), LEFT_TRIANGLE);
	}

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

	m_init(TRUE);
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	instance->confirm = m_confirm_impl;
	return instance;
}