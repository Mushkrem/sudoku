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
    int (*is_legacy)();
    int _is_legacy;
    _menu *menu;
} _screen;
