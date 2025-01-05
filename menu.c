
#include "menu.h"
#include "utils.h"
#include "screen.h"
#include "game.h"
#include "interface.h"

#define Y_OFFSET 4
#define DEFAULT_BOUNDS (_bounds){0, 0}
#define MAX_LABELS 32

#define LEFT_TRIANGLE screen->is_legacy() == 1 ? L"\u25C4" : L"\u25C0"

_menu* instance = NULL;

_menu* get_menu_instance(_bounds bounds);
_screen* get_screen_instance(_game* game_ref);

_bounds bounds;
_game* game_ref;
const wchar_t* buttons[10];
int is_dynamic[10] = { 0 };
int button_count = 0;

void add_button(wchar_t* string) {
	buttons[button_count] = string;
	is_dynamic[button_count++] = 0;
}

void add_buttons(wchar_t* first, ...) {
	va_list args;
	va_start(args, first);

	wchar_t* current_string = first;
	while (current_string != NULL) {
		add_button(current_string);
		current_string = va_arg(args, wchar_t*);
	}

	va_end(args);
}

void remove_buttons() {
	for (int i = 0; buttons[i] != NULL; i++) {
		if (is_dynamic[i]) free(buttons[i]);
		buttons[i] = NULL;
		is_dynamic[i] = 0;
		button_count = 0;
	}
}

void m_init(int first_run) {
	_screen* screen = get_screen_instance(NULL);
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	if (first_run) {
		menu->position = -1;
		menu->previous_position = -1;
		
		game_ref = screen->get_game_ref();

		wchar_t buffer[64];
		Utils.write_literal(buffer, sizeof(buffer), L"\033[7m\033[9%dm S U D O K U ", screen->get_theme());

		menu->labels[0].string = _wcsdup(buffer);
		menu->labels[0].position = (COORD){ bounds.width / 2 - 6, Y_OFFSET - 2 };
	}

	add_buttons(L"Play", L"Options", L"Exit", NULL);

	if (!first_run) menu->position = button_count;
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
	if (Utils.contains(buttons[p - 1], L"Grid")) game_ref->set_grid(game_ref->grid + direction * 3);
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
	int p_buttons = button_count;

	if (buttons[p - 1] == L"Exit") exit(EXIT_SUCCESS);

	if (buttons[p - 1] == L"Back") {
		remove_buttons();
		m_init(FALSE);
	}

	if (buttons[p - 1] == L"Continue") { // Start the actual game
		menu->remove_labels();
		remove_buttons();

		system("cls");

		game_ref->start();
		return EXIT_SUCCESS;
	}

	if (buttons[p - 1] == L"Options") { // Create the 'Options' menu
		instance->position = 1;
		remove_buttons();
		add_buttons(L"[%s] Theme", L"", L"", L"Back", NULL);
	}

	if (buttons[p - 1] == L"Play") { // Create the 'Play' menu
		instance->position = 1;
		remove_buttons();
		add_buttons(L"Continue", L"", L"[%s] Difficulty", L"[%s] Grid", L"", L"Back", NULL);
	}

	if (p_buttons != button_count) {
		system("cls"); // This is fine as it's used sporadically, so doesn't seem to cause much tearing
		m_update_impl(); // Update the menu
	}

	return EXIT_SUCCESS;
}

void m_draw_buttons() {
	for (int i = 0; i < button_count; i++) {
		wchar_t* higher_intensity = L"\033[1m"; // makes all buttons higher intensity

		if (instance->position != i + 1) // un-intesifies buttons that aren't selected
			higher_intensity = L"";

		if (Utils.contains(buttons[i], L"]")) { // Dynamic buttons (with variables inside of them)
			wchar_t temp_buffer[64] = { 0 };
			_screen* screen = get_screen_instance(NULL);

			if (Utils.contains(buttons[i], L"Theme"))
				Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"[\033[9%dm%d\033[39m] Theme", screen->get_theme(), screen->get_theme());

			if (Utils.contains(buttons[i], L"Difficulty"))
				Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"  [\033[9%dm%s\033[39m] Difficulty  ", screen->get_theme(), game_ref->get_difficulty());

			if (Utils.contains(buttons[i], L"Grid"))
				Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"[\033[9%dm%s\033[39m] Grid", screen->get_theme(), game_ref->get_grid());

			if (is_dynamic[i]) {
				free(buttons[i]);
			}

			buttons[i] = _wcsdup(temp_buffer);
			is_dynamic[i] = 1;
		}

		Interface.draw_text((COORD) { bounds.width / 2 - Utils.wcslen(buttons[i])/2, (bounds.height / 2) - 2 + i * 2 }, L"%s%s", higher_intensity, buttons[i]);
	}
}

void m_remove_labels() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	for (int i = 0; menu->labels[i].string != NULL; i++) {
		menu->labels[i] = (_label){ NULL, (COORD) { 0, 0 } };
	}
}

void m_draw_labels() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	for (int i = 0; menu->labels[i].string != NULL; i++) {
		_label label = menu->labels[i];
		Interface.draw_text(label.position, label.string);
	}
}

int m_update_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	_screen* screen = get_screen_instance(NULL);

	int p = instance->previous_position;
	int n = instance->position;

	m_draw_buttons(); // drawing buttons before drawing cursor, because some buttons are dynamic and their length may change
	m_draw_labels(); // drawing all other short texts

	if (p <= button_count) { // Removing the previous cursor
		Interface.draw_text((COORD) { bounds.width / 2 + Utils.wcslen(buttons[p - 1]) / 2 + 5, (bounds.height / 2) - 4 + (p) * 2 }, L" ");
	}

	if (n <= button_count) { // Drawing the cursor, it's x position depending on the length of the selected button.
		Interface.draw_text((COORD) { bounds.width / 2 + Utils.wcslen(buttons[n - 1])/2 + 5, (bounds.height / 2) - 4 + (n) * 2 }, L"\033[6m\033[9%dm%s", screen->get_theme(), LEFT_TRIANGLE);
	}

	return EXIT_SUCCESS;
}

void m_update_label_impl(_label* label, const wchar_t* string, COORD position) {
	if (label->string != NULL) {
		free(label->string);
	}
	label->string = _wcsdup(string);
	label->position = position;
}

_menu* get_menu_instance(_bounds b) {
	if (instance != NULL) {
		return instance;
	}

	instance = (_menu*)malloc(sizeof(_menu)); // allocate memory for the new instance
	if (instance == NULL)
		exit(EXIT_FAILURE);

	if (b.width != 0) {
		bounds.width = b.width;
	}

	// allocate memory for the labels array on the heap
	instance->labels = (_label*)malloc(MAX_LABELS * sizeof(_label));
	if (instance->labels == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; i < MAX_LABELS; i++) {
		instance->labels[i].string = NULL;
		instance->labels[i].position = (COORD){ 0, 0 };
	}

	m_init(TRUE);
	instance->update_label = m_update_label_impl;
	instance->remove_labels = m_remove_labels;
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	instance->confirm = m_confirm_impl;
	
	return instance;
}