#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _game {
    void (*set_grid)(int n);
    void (*set_difficulty)(int n);
    wchar_t* (*get_difficulty)();
    wchar_t* (*get_grid)();
    void (*start)();

    int grid;
    int points;
    int difficulty;
    int started;
} _game;

void g_init(_game* g);