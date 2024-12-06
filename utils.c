#include "utils.h"

void output_formatted_debug(const wchar_t* format, ...) {
	wchar_t buffer[2048];
	va_list args;
	va_start(args, format);
	vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
	va_end(args);

	wcscat_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"\n");
	OutputDebugString(buffer);
}

void write_to_buffer(wchar_t* buffer, size_t size, const wchar_t* format, va_list args) {
	int length = vswprintf(buffer, size, format, args);
}

void write_to_console(const wchar_t* buffer, COORD position) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, position);
	DWORD c_written; 
	if (h != INVALID_HANDLE_VALUE) { 
		WriteConsoleW(h, buffer, wcslen(buffer), &c_written, NULL); 
	}
}

Utility Utils = {
	.Debug = output_formatted_debug,
	.Write = write_to_buffer,
	.Print = write_to_console
};