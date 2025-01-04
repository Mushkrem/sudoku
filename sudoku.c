#pragma once

#include "sudoku.h"
#include "screen.h"
#include "input.h"
#include "utils.h"

//  ┌─────────┬─────────┬────────┐
//  ├─────────┼─────────┼────────┤
//  │         │         │        │
//  ├─────────┼─────────┼────────┤
//  └─────────┴─────────┴────────┘

_screen* get_screen_instance(_game* game_ref);
_game game;
wchar_t* difficulties[3] = { L"Easy", L"Normal", L"Hard" };

void set_grid(int n) {
    if (n < 1) n = 5;
    if (n > 5) n = 1;
    game.grid = n;
}

void set_difficulty(int n) {
    if (n < 1) n = 3;
    if (n > 3) n = 1;
    game.difficulty = n;
}

wchar_t* get_difficulty() {
    return difficulties[game.difficulty - 1];
}

wchar_t* get_grid() {
    wchar_t buffer[32];
    Utils.write_literal(buffer, sizeof(buffer), L"%dx%d", game.grid, game.grid);
    
    wchar_t* result = _wcsdup(buffer);
    return result;
}

int main(int argc, char* argv[]) {
    // Initializing input and screen loop functions
    INPUTS input = get_inputs();
    
    game.difficulty = 1;
    game.points = 0;
    game.grid = 3;

    game.set_grid = set_grid;
    game.get_grid = get_grid;
    game.set_difficulty = set_difficulty;
    game.get_difficulty = get_difficulty;

    _screen* screen = get_screen_instance(&game);
    
    // Creating parallel threads
    HANDLE listen_t, screen_t;
    int thread_number = 1;

    listen_t = CreateThread(NULL, 0, input.listen, &thread_number, 0, NULL);
    screen_t = CreateThread(NULL, 0, screen->update, &thread_number, 0, NULL);

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