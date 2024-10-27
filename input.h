#pragma once
#include <windows.h>
#include <conio.h>

typedef struct _input {
    int (*listen)(LPVOID lp);
} INPUTS;

INPUTS get_inputs();
