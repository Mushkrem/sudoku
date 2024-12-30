#include "utils.h"

void append_to_buffer(wchar_t* buffer, size_t size, const wchar_t* string) {
	wcscat_s(buffer, size / sizeof(wchar_t), string);
}

void write_to_buffer(wchar_t* buffer, size_t size, const wchar_t* string, va_list args) {
	vswprintf(buffer, size, string, args);
}

void output_formatted_debug(const wchar_t* string, ...) {
	wchar_t buffer[2048];
	va_list args;
	va_start(args, string);
	vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), string, args);
	va_end(args);

	append_to_buffer(buffer, sizeof(buffer), L"\n");
	OutputDebugString(buffer);
}

void write_to_console(const wchar_t* buffer, COORD position) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);
	DWORD c_written; 
	if (handle != INVALID_HANDLE_VALUE) {
		WriteConsoleW(handle, buffer, wcslen(buffer), &c_written, NULL);
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
	.Debug = output_formatted_debug,
	.Write = write_to_buffer,
	.Print = write_to_console,
	.Append = append_to_buffer,
	.GetCharacterAt = get_character_at_position,
	.GetConsoleWidth = get_console_width,
	.GetConsoleHeight = get_console_height
};