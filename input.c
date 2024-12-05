#pragma once

#include "input.h"
#include "screen.h"

#include "utils.h"

static DWORD WINAPI listen_impl(LPVOID lp) {
    int* number = (int*)lp;
    char c;
    int d;

    SCREEN screen;
    screen = get_screen();

    while (TRUE) {
        c = _getch();

        if(iswdigit(c)) // isdigit threw exceptions in some cases
            screen.select(c - '0');
    }

    return 0;
}

INPUTS get_inputs() {
    INPUTS input;
    input.listen = listen_impl;

    return input;
}