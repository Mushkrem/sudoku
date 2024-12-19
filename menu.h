#pragma once

#include "interface.h"

typedef struct _menu {
    unsigned char position;
    unsigned char previous_position;
    _label** labels;
    int visible;
    int (*update)();
    int (*select)();
} _menu;