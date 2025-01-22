#pragma once
#include <stdio.h>
#include <windows.h>

#include "game.h"
#include "menu.h"

typedef struct {
    int width, height;
} _bounds;

typedef struct _screen {
    int (*update)();
    int (*confirm)();
    int (*is_legacy)();
    int (*get_theme)();
    int (*get_draw_empty)();
    int (*select)(int n);
    void (*set_theme)(int n);
    void (*set_draw_empty)(int n);
    _game* (*get_game_ref)();

    int _is_legacy;
    _menu *menu;
} _screen;
