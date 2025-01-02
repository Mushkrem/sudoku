#pragma once

#include "interface.h"

typedef struct _menu {
    unsigned char position;
    unsigned char previous_position;
    _label** labels;
    int theme;
    int (*update)();
    int (*select)();
    int (*confirm)();
} _menu;