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
    void (*update)(int from_selection);
    void (*start)();
    void (*stop)();
    void (*timer_t)();
    int (*select)(int n);

    int grid;
    int points;
    int difficulty;
    int started;
    int mistakes;
    int elapsed;
    int total_elapsed;
    int reset_timer;
    int score;
} _game;

void g_init(_game* g);