#pragma once

#include "game.h"
#include "screen.h"
#include "input.h"
#include "utils.h"

#include <time.h>

//  ┌─────────┬─────────┬────────┐
//  ├─────────┼─────────┼────────┤
//  │         │         │        │
//  ├─────────┼─────────┼────────┤
//  └─────────┴─────────┴────────┘

_screen* get_screen_instance(_game* game_ref);

int main(int argc, char* argv[]) {
    _game game;
    
    game.difficulty = 1;
    game.points = 0;
    game.grid = 9;

    g_init(&game);

    _screen* screen = get_screen_instance(&game);

    // Initializing input and screen loop functions
    INPUTS input = get_inputs();

    // Creating parallel threads
    HANDLE listen_t, screen_t;
    int thread_number = 1;

    screen_t = CreateThread(NULL, 0, screen->update, &thread_number, 0, NULL);
    listen_t = CreateThread(NULL, 0, input.listen, &thread_number, 0, NULL);

    if (listen_t == NULL || screen_t == NULL) {
        return EXIT_FAILURE;
    }

    // Initialize the menu

    screen->select(1);

    WaitForSingleObject(listen_t, INFINITE);
    
    // Closing the program
    
    CloseHandle(listen_t);
    CloseHandle(screen_t);


    return EXIT_SUCCESS;
}