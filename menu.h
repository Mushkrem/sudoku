#pragma once

#include "interface.h"

typedef struct _label {
    wchar_t* string;
    COORD position;
} _label;

typedef struct _menu {
    int theme;
    _label* labels;
    unsigned char position;
    unsigned char previous_position;

    int (*update)();
    int (*select)();
    int (*confirm)();
    void (*update_label)();

} _menu;