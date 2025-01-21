#pragma once

#include "input.h"
#include "screen.h"
#include "game.h"

#include "utils.h"

_screen* get_screen_instance();

_game* game_ref;

static DWORD WINAPI listen_impl(LPVOID lp) {
    int* number = (int*)lp;
    char c;
    int d;

    _screen* screen = get_screen_instance();

    while (TRUE) {
        c = _getch();

        if (iswdigit(c)) // isdigit threw exceptions in some cases
            screen->select(c - '0' + 0xFFF);
           
        switch ((int)c) {
            case 72: // up
                screen->select(screen->menu->position - 1);
                break;
            case 80: // down
                screen->select(screen->menu->position + 1);
                break;
            case 77: // right
                screen->select(screen->menu->position + 0xA);
                break;
            case 75: // left
                screen->select(screen->menu->position + 0xF);
                break;
            case 13: // enter
                screen->confirm();
                break;
            case 27: // esc
                if (!game_ref->started) break;
                game_ref->stop();
                screen->menu->init(TRUE);
                screen->select(1);
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

    _screen* screen = get_screen_instance();
    game_ref = screen->get_game_ref();

    return input;
}