#pragma once

#include "input.h"
#include "screen.h"

static DWORD WINAPI listen_impl(LPVOID lp) {
    int* number = (int*)lp;
    char c;
    int d;

    SCREEN screen;
    screen = get_screen();

    screen.select(5);

    // main input loop
    while (TRUE) {
        c = _getch();
        if(isdigit(c))
            screen.select(c - '0');
    }

    return 0;
}

INPUTS get_inputs() {
    INPUTS input;
    input.listen = listen_impl;

    return input;
}