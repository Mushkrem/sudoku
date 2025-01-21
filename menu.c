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
int label_count = 0;

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

		menu->remove_labels();

		system("cls");
		
		game_ref = screen->get_game_ref();

		wchar_t buffer[64];
		Utils.write_literal(buffer, sizeof(buffer), L"[] Sudoku");

		menu->update_label(&menu->labels[0], buffer, (COORD) { bounds.width / 2 - 6, Y_OFFSET - 2 });

	}

	add_buttons(L"Play", L"Options", L"Exit", NULL);

	menu->update();

	if (!first_run) menu->position = button_count;
}

int m_roll_impl(int n) {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	_screen* screen = get_screen_instance(NULL);
	
	if (button_count == 0) return EXIT_FAILURE;

	int direction = 0;
	int p = menu->position;

	if (n - p == 0xA) direction = 1;
	if (n - p == 0xF) direction = -1;

	if (direction == 0) return !EXIT_FAILURE;

	if (Utils.contains(buttons[p - 1], L"Theme")) screen->set_theme(screen->get_theme() + direction);
	if (Utils.contains(buttons[p - 1], L"Grid")) game_ref->set_grid(game_ref->grid + direction * 3);
	if (Utils.contains(buttons[p - 1], L"Difficulty")) game_ref->set_difficulty(game_ref->difficulty + direction);
	
	menu->update();

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
		menu->position = 0;
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
		free(menu->labels->string);
	}
}

void m_draw_labels() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	_screen* screen = get_screen_instance(NULL);
	for (int i = 0; menu->labels[i].string != NULL; i++) {
		_label label = menu->labels[i];
		
		wchar_t temp_buffer[128] = { 0 };

		if (Utils.contains(label.string, L"Sudoku")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"\033[7m\033[9%dm S U D O K U ", screen->get_theme());
			label.position = (COORD){ bounds.width / 2 - 6, Y_OFFSET - 2 };
		}

		else if (Utils.contains(label.string, L"ESC_QUIT")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"\033[9%dmESC\033[39m Quit", screen->get_theme());
			label.position = (COORD){ 2, 0 };
		}

		else if (Utils.contains(label.string, L"NO_PLACE")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"\033[9%dm0\033[39m-\033[9%dm%d\033[39m Place", screen->get_theme(), screen->get_theme(), game_ref->grid);
			label.position = (COORD){ 2, bounds.height - 3 };
		}

		else if (Utils.contains(label.string, L"UP_DOWN")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"\033[9%dm↑\033[39m/\033[9%dm↓\033[39m Up/Down", screen->get_theme(), screen->get_theme());
			label.position = (COORD){ 2, bounds.height - 2 };
		}

		else if (Utils.contains(label.string, L"RIGHT_LEFT")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), L"\033[9%dm→\033[39m/\033[9%dm←\033[39m Right/Left", screen->get_theme(), screen->get_theme());
			label.position = (COORD){ 2, bounds.height - 1 };
		}

		else if (Utils.contains(label.string, L"ELAPSED")) {
			int minutes = game_ref->elapsed / 60;
			int seconds = game_ref->elapsed % 60;

			if (minutes > 0) {
				Utils.write_literal(temp_buffer, sizeof(temp_buffer), 
					L"Time elapsed: \033[9%dm%02d\033[39mm \033[9%dm%02d\033[39ms", 
					screen->get_theme(), minutes, screen->get_theme(), game_ref->elapsed - minutes * 60);
			}
			else {
				Utils.write_literal(temp_buffer, sizeof(temp_buffer), 
					L"Time elapsed: \033[9%dm%02d\033[39ms", 
					screen->get_theme(), game_ref->elapsed);
			}
			label.position = (COORD){ bounds.width - Utils.wcslen(temp_buffer), bounds.height - 1 };
		}

		else if (Utils.contains(label.string, L"MISTAKES")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), 
				L"Mistakes: \033[9%dm%d", 
				screen->get_theme(), game_ref->mistakes);
			label.position = (COORD){ bounds.width - Utils.wcslen(temp_buffer), bounds.height - 2};
		}

		else if (Utils.contains(label.string, L"CONGRATULATIONS")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), 
				L"\033[7m\033[9%dm C O N G R A T U L A T I O N S ", 
				screen->get_theme());
			label.position = (COORD){ (bounds.width / 2) - (Utils.wcslen(temp_buffer) / 2), 4 };
		}

		else if (Utils.contains(label.string, L"STAT_MISS")) {
			Utils.write_literal(temp_buffer, sizeof(temp_buffer), 
				L"Total mistakes: \033[9%dm%d", 
				screen->get_theme(), game_ref->mistakes);
			label.position = (COORD){ (bounds.width / 2) - (Utils.wcslen(temp_buffer) / 2), bounds.height - 3 };
		}

		else if (Utils.contains(label.string, L"STAT_TIME")) {
			int minutes = game_ref->total_elapsed / 60;
			Utils.write_literal(temp_buffer, sizeof(temp_buffer),
				L"Time elapsed: \033[9%dm%02d\033[39mm \033[9%dm%02d\033[39ms",
				screen->get_theme(), minutes, screen->get_theme(), game_ref->total_elapsed - minutes * 60);
			label.position = (COORD){ (bounds.width / 2) - (Utils.wcslen(temp_buffer) / 2), bounds.height - 2 };
		}

		else if (Utils.contains(label.string, L"STAT_DIFF")) {
			int spacing = (game_ref->grid % 2 == 0) ? 2 : 3;
			int board_size_y = game_ref->grid + (int)(game_ref->grid / 3) + (3 - spacing);
			int y_offset = Utils.get_console_height() / 2 - board_size_y / 2;

			int minutes = game_ref->total_elapsed / 60;
			Utils.write_literal(temp_buffer, sizeof(temp_buffer),
				L"Difficulty: \033[9%dm%s",
				screen->get_theme(), game_ref->get_difficulty());
			label.position = (COORD){ (bounds.width / 2) - (Utils.wcslen(temp_buffer) / 2), y_offset - 2 };
		}

		else if (Utils.contains(label.string, L"SCORE")) {
			int spacing = (game_ref->grid % 2 == 0) ? 2 : 3;
			int board_size_y = game_ref->grid + (int)(game_ref->grid / 3) + (3 - spacing);
			int y_offset = Utils.get_console_height() / 2 - board_size_y / 2;

			Utils.write_literal(temp_buffer, sizeof(temp_buffer),
				L"Score: \033[9%dm%d",
				screen->get_theme(), game_ref->score);
			label.position = (COORD){ (bounds.width / 2) - (Utils.wcslen(temp_buffer) / 2), y_offset + board_size_y + 2 };
		}

		else Utils.write_literal(temp_buffer, sizeof(temp_buffer), label.string);

		label.string = _wcsdup(temp_buffer);

		Interface.draw_text(label.position, label.string);
	}
}

int m_update_impl() {
	_menu* menu = get_menu_instance(DEFAULT_BOUNDS);
	_screen* screen = get_screen_instance(NULL);
	_bounds current_bounds = (_bounds){ Utils.get_console_width(), Utils.get_console_height() };
	if (current_bounds.height != bounds.height || current_bounds.width != bounds.width) {
		bounds = current_bounds;
	}

	int p = instance->previous_position;
	int n = instance->position;
	
	m_draw_labels(); // drawing all other short texts

	if (game_ref->started) return EXIT_SUCCESS;

	m_draw_buttons(); // drawing buttons before drawing cursor, because some buttons are dynamic and their length may change

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

	m_draw_labels();
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

	instance->update_label = m_update_label_impl;
	instance->remove_labels = m_remove_labels;
	instance->update = m_update_impl;
	instance->select = m_select_impl;
	instance->confirm = m_confirm_impl;
	instance->init = m_init;

	m_init(TRUE);
	
	return instance;
}