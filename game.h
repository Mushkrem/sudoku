#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _game {
    void (*set_difficulty)(int n);
    wchar_t* (*get_difficulty)();
    void (*set_grid)(int n);
    wchar_t* (*get_grid)();
    void (*update)();
    void (*start)();
    void (*stop)();

    int grid;
    int points;
    int difficulty;
    int started;
} _game;

void g_init(_game* g);