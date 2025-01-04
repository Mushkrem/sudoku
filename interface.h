#pragma once

#include <windows.h>
#include <wchar.h>
#include <stdarg.h>

typedef struct UINTERFACE {
	int (*read_resources)();
    int (*draw_text)(COORD position, const wchar_t*, ...);
} UINTERFACE;

extern UINTERFACE Interface;