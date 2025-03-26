#include "utils.h"

int string_contains(const wchar_t* first, const wchar_t* second) {
	return wcsstr(first, second) != NULL;
}

size_t wcslen_no_ansi(const wchar_t* string) {
	size_t length = 0;
	int in_escape = 0;

	while (*string) {
		if (*string == L'\033') {
			in_escape = 1;
		}
		else if (in_escape && *string == L'm') {
			in_escape = 0;
		}
		else if (!in_escape) {
			length++;
		}
		string++;
	}

	return length;
}

void clear_console() { // unused as of now
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

void append_to_buffer(wchar_t* buffer, size_t size, const wchar_t* string) {
	wcscat_s(buffer, size / sizeof(wchar_t), string);
}

void write_to_buffer(wchar_t* buffer, size_t size, const wchar_t* string, va_list args) {
	vswprintf(buffer, size / sizeof(wchar_t), string, args);
}

void write_to_buffer_literally(wchar_t* buffer, size_t size, const wchar_t* string, ...) {
	va_list args;
	va_start(args, string);
	write_to_buffer(buffer, size / sizeof(wchar_t), string, args);
	va_end(args);
}

// void output_formatted_debug(const wchar_t* string, ...) {
// 	wchar_t buffer[2048];
// 	va_list args;
// 	va_start(args, string);
// 	vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), string, args);
// 	va_end(args);

// 	append_to_buffer(buffer, sizeof(buffer), L"\n");
// 	OutputDebugString(buffer);
// }

void write_to_console(const wchar_t* buffer, COORD position) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);
	DWORD c_written; 
	if (handle != INVALID_HANDLE_VALUE) {
		wchar_t clear_char = L' ';
		WriteConsoleW(handle, &clear_char, 1, &c_written, NULL);

		SetConsoleCursorPosition(handle, position);

		WriteConsoleW(handle, buffer, (DWORD)wcslen(buffer), &c_written, NULL);
	}
}

char get_character_at_position(COORD pos) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CHAR_INFO screen_buffer[1];

	COORD buffer_size = { 1, 1 };
	COORD buffer_coord = { 0, 0 };
	SMALL_RECT read_region = { pos.X, pos.Y, pos.X, pos.Y };

	int found = ReadConsoleOutput(handle, screen_buffer, buffer_size, buffer_coord, &read_region);

	if (found) return screen_buffer->Char.AsciiChar;
	return EXIT_FAILURE;
}

int get_console_width() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int width;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	return width;
}

int get_console_height() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int height;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	height = csbi.srWindow.Bottom - csbi.srWindow.Top;

	return height;
}

Utility Utils = {
	// .debug = output_formatted_debug,
	.write = write_to_buffer,
	.write_literal = write_to_buffer_literally,
	.print = write_to_console,
	.contains = string_contains,
	.wcslen = wcslen_no_ansi,
	.append = append_to_buffer,
	.get_character_at = get_character_at_position,
	.get_console_width = get_console_width,
	.get_console_height = get_console_height
};
