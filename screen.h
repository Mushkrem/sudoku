#pragma once
#include <stdio.h>
#include <windows.h>

#include "menu.h"

typedef struct _screen {
    int (*update)();
    int (*select)(int n);
    _menu *menu;
} SCREEN;

SCREEN get_screen();