#pragma once

#include <windows.h>
#include <wchar.h>
#include <stdarg.h>

typedef struct _label {
    unsigned char bit;
    int weight;
    char* text;
} _label;

typedef struct UINTERFACE {
	int (*read_resources)();
    int (*draw_text)(COORD position, const wchar_t*, ...);
} UINTERFACE;

extern UINTERFACE Interface;