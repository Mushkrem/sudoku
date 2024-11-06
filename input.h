#pragma once
#include <windows.h>

typedef struct _input {
    int (*listen)(LPVOID lp);
} INPUTS;

INPUTS get_inputs();
