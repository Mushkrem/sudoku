#pragma once

#include <windows.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {
	void (*Debug)(const wchar_t* format, ...);
	void (*Write)(wchar_t* buffer, size_t size, const wchar_t* format, ...);
	void (*Print)(wchar_t* buffer, COORD position);
} Utility;

extern Utility Utils;