#pragma once

#include <windows.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {
	void (*Debug)(const wchar_t* string, ...);
	void (*Write)(wchar_t* buffer, size_t size, const wchar_t* string, ...);
	void (*Append)(wchar_t* buffer, size_t size, const wchar_t* string);
	void (*Print)(wchar_t* buffer, COORD position);
	int (*GetConsoleWidth)();
} Utility;

extern Utility Utils;