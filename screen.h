#pragma once
#include <stdio.h>
#include <windows.h>

#include "menu.h"

typedef struct _screen {
    int (*update)();
    int (*confirm)();
    int (*select)(int n);
    _menu *menu;
    int width;
} SCREEN;

SCREEN get_screen();