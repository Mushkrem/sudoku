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
    int (*get_theme)();
    int (*set_theme)(int n);
    int _is_legacy;
    _menu *menu;
} _screen;
