#pragma once
#include <stdio.h>
#include <windows.h>

#include "interface.h"

typedef struct _menu {
    unsigned char position;
    _label** labels;
} _menu;

typedef struct _screen {
    int (*update)();
    int (*select)(int n);
    _menu* menu;
} SCREEN;

SCREEN get_screen();