#pragma once

#include "input.h"

DWORD WINAPI listen_impl(LPVOID lp) {
    int* number = (int*)lp;
    char c;

    while (TRUE) {
        c = _getch();
        printf("%c", c);
    }

    return 0;
}

INPUTS get_inputs() {
    INPUTS input;
    input.listen = listen_impl;

    return input;
}