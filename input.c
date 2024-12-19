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
           
        switch ((int)c) {
            case 72: // up
                screen.select(screen.menu->position - 1);
                break;
            case 80: // down
                screen.select(screen.menu->position + 1);
                break;
            case 77: // right
                break;
            case 75: // left
                break;
            case 13: // enter
                screen.confirm();
                break;
            default:
                break;
        }
    }

    return 0;
}

INPUTS get_inputs() {
    INPUTS input;
    input.listen = listen_impl;

    return input;
}