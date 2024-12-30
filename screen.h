#pragma once
#include <stdio.h>
#include <windows.h>

#include "menu.h"

typedef struct {
    int width, height;
} _bounds;

typedef struct _screen {
    int (*update)();
    int (*confirm)();
    int (*select)(int n);
    _menu *menu;
} SCREEN;

//SCREEN get_screen();