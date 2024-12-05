#pragma once

#include <windows.h>
#include <wchar.h>
#include <stdarg.h>

typedef struct {
	void (*Debug)(const wchar_t* format, ...);
} Utility;

extern Utility Utils;