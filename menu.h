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

    void (*init)();
    int (*update)();
    int (*select)();
    int (*confirm)();
    void (*update_label)();
    void (*remove_labels)();

} _menu;