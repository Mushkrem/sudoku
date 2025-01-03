#pragma once
#include <stdio.h>
#include <windows.h>

#include "sudoku.h"
#include "menu.h"

typedef struct {
    int width, height;
} _bounds;

typedef struct _screen {
    int (*update)();
    int (*confirm)();
    int (*is_legacy)();
    int (*get_theme)();
    int (*select)(int n);
    int (*set_theme)(int n);
    _game* (*get_game_ref)();

    int _is_legacy;
    _menu *menu;
} _screen;
