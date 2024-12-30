#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "screen.h"
#include "input.h"
#include "utils.h"

typedef struct _global {
    int points;
    int position;
} _global;

typedef struct _game {
    int points;
    _global* global_ref;
} _game;

//  ┌─────────┬─────────┬────────┐
//  ├─────────┼─────────┼────────┤
//  │         │         │        │
//  ├─────────┼─────────┼────────┤
//  └─────────┴─────────┴────────┘

_screen* get_screen_instance();

int main(int argc, char* argv[]) {
    // Initializing input and screen loop functions
    INPUTS input = get_inputs();
    _screen* screen = get_screen_instance();
    
    _global global;
    _game game;
    Utils.Print(L"┌─────────┬─────────┬────────┐", (COORD) { 0, 12 });
    game.global_ref = &global;

    // Creating parallel threads
    HANDLE listen_t, screen_t;
    int thread_number = 1;


    listen_t = CreateThread(NULL, 0, input.listen, &thread_number, 0, NULL);
    screen_t = CreateThread(NULL, 0, screen->update, &thread_number, 0, NULL);

    if (listen_t == NULL || screen_t == NULL) {
        return EXIT_FAILURE;
    }

    // Initialize the menu

    //screen->menu->select(1);
    screen->select(1);


    WaitForSingleObject(listen_t, INFINITE);
    
    // Closing the program
    
    CloseHandle(listen_t);
    CloseHandle(screen_t);


    return EXIT_SUCCESS;
}