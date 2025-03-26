#pragma once

#include <windows.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {
	// void (*debug)(const wchar_t* string, ...);
	void (*write)(wchar_t* buffer, size_t size, const wchar_t* string, va_list args);
	void (*write_literal)(wchar_t* buffer, size_t size, const wchar_t* string, ...);
	int (*contains)(wchar_t* first, wchar_t* second);
	size_t (*wcslen)(wchar_t* string);
	void (*append)(wchar_t* buffer, size_t size, const wchar_t* string);
	void (*print)(wchar_t* buffer, COORD position);
	char (*get_character_at)(COORD position);
	int (*get_console_width)();
	int (*get_console_height)();
} Utility;

extern Utility Utils;
