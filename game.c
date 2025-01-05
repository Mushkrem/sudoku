#pragma once

#include "game.h"
#include "utils.h"
#include "interface.h"
#include "screen.h"

_game* game;
wchar_t* difficulties[3] = { L"Easy", L"Normal", L"Hard" };
_screen* get_screen_instance(_game* game_ref);

int solved_board[9][9] = {              //  ┌─────────┬─────────┬─────────┐
    {5, 3, 4,   6, 7, 8,    9, 1, 2},   //  │ 5  3  4 │ 6  7  8 │ 9  1  2 │
    {6, 7, 2,   1, 9, 5,    3, 4, 8},   //  │ 6  7  2 │ 1  9  5 │ 3  4  8 │
    {1, 9, 8,   3, 4, 2,    5, 6, 7},   //  │ 1  9  8 │ 3  4  2 │ 5  6  7 │
                                        //  ├─────────┼─────────┼─────────┤
    {8, 5, 9,   7, 6, 1,    4, 2, 3},   //  │ 8  5  9 │ 7  6  1 │ 4  2  3 │
    {4, 2, 6,   8, 5, 3,    7, 9, 1},   //  │ 4  2  6 │ 8  5  3 │ 7  9  1 │
    {7, 1, 3,   9, 2, 4,    8, 5, 6},   //  │ 7  1  3 │ 9  2  4 │ 8  5  6 │
                                        //  ├─────────┼─────────┼─────────┤
    {9, 6, 1,   5, 3, 7,    2, 8, 4},   //  │ 9  6  1 │ 5  3  7 │ 2  8  4 │
    {2, 8, 7,   4, 1, 9,    6, 3, 5},   //  │ 2  8  7 │ 4  1  9 │ 6  3  5 │
    {3, 4, 5,   2, 8, 6,    1, 7, 9}    //  │ 3  4  5 │ 2  8  6 │ 1  7  9 │
};                                      //  └─────────┴─────────┴─────────┘

int board[9][9];

void generate_board() { }

void draw_board() {
    _screen* screen = get_screen_instance(NULL);
    int board_size_y = game->grid + (int)(game->grid / 3);
    int board_size_x = game->grid * 2 + (int)(game->grid / 3)*2;
    int y_offset = Utils.get_console_height() / 2 - board_size_y / 2;
    int x_offset = Utils.get_console_width() / 2 - board_size_x / 2;
    int x_inc = 0; // additional spacing for vertical borders between the numbers
    int y_inc = 0; // additional spacing for horizontal borders between the numbers

    int is_odd = (game->grid % 2 == 0) ? 0 : 1;

    for (int column = 0; column <= board_size_x; column++) { // <= so the border is bigger
        for (int row = 0; row <= board_size_y; row++) {
            

            if (column == 0) { // Left side
                Interface.draw_text((COORD) { x_offset, y_offset + row }, L"│", screen->get_theme());
            }

            if ((row + 1) % 3 == 0 && column < board_size_x) { // Inner table
                if (column == 0 && row != 2 && row != board_size_y - is_odd)
                    Interface.draw_text((COORD) { x_offset, y_offset - 2 + y_inc }, L"├");

                if (column == board_size_x - 1 && row != 2 && row != board_size_y - 1)
                    Interface.draw_text((COORD) { x_offset + board_size_x, y_offset - 2 + y_inc }, L"┤");

                if ((column + 1) % 6 == 0 && column < board_size_x - 1) {
                    Interface.draw_text((COORD) { x_offset + x_inc + 2, y_offset - 2 + y_inc }, L"┼");
                    if (row == 2) Interface.draw_text((COORD) { x_offset + x_inc + 2, y_offset - 2 + y_inc }, L"┬");
                    if (row == board_size_y - is_odd) Interface.draw_text((COORD) { x_offset + x_inc + 2, y_offset - 2 + y_inc }, L"┴");
                }

                if (column % 8 != 0) {
                    Interface.draw_text((COORD) { x_offset + column, y_offset - 2 + y_inc }, L"─");
                    if (game->grid == 3) Interface.draw_text((COORD) { x_offset + column, y_offset + 2 + y_inc }, L"─");
                }

            }

            if (row == 0) {
                if (column == 0) Interface.draw_text((COORD) { x_offset, y_offset }, L"┌");
                if (column == board_size_x) Interface.draw_text((COORD) { x_offset + board_size_x, y_offset }, L"┐");
            }

            if (row == board_size_y) {
                if (column == 0) Interface.draw_text((COORD) { x_offset, y_offset + board_size_y }, L"└");
                if (column == board_size_x) Interface.draw_text((COORD) { x_offset + board_size_x, y_offset + board_size_y }, L"┘");
            }

            if (row < game->grid && column < game->grid) {
                int n = solved_board[column][row];
                if (n != 0) {
                    Interface.draw_text((COORD) { x_offset + 2 + x_inc * 2, y_offset + 1 + y_inc }, L"%x", n);
                }
                else {
                    Interface.draw_text((COORD) { x_offset + 2 + x_inc * 2, y_offset + 1 + y_inc }, L"┄");
                }
                if ((column + 1) % 3 == 0)
                    Interface.draw_text((COORD) { x_offset + 2 + x_inc * 2 + 2, y_offset + 1 + y_inc }, L"│");
            }

            if ((row + 1) % 3 == 0) 
                y_inc++;

            y_inc++;
        }
        y_inc = 0;
        x_inc++;

        if ((column+1) % 3 == 0) x_inc++;
    }
}

void set_grid(int n) {
    if (n < 3) n = 9;
    if (n > 9) n = 3;
    game->grid = n;
}

void set_difficulty(int n) {
    if (n < 1) n = 3;
    if (n > 3) n = 1;
    game->difficulty = n;
}

wchar_t* get_difficulty() {
	return difficulties[game->difficulty - 1];
}

wchar_t* get_grid() {
    wchar_t buffer[32];
    Utils.write_literal(buffer, sizeof(buffer), L"%dx%d", game->grid, game->grid);

    wchar_t* result = _wcsdup(buffer);
    return result;
}

void start() {
    game->started = 1;
    draw_board();
}

void g_init(_game* g) {
	game = g;
	
	game->get_difficulty = get_difficulty;
    game->set_difficulty = set_difficulty;
	game->get_grid = get_grid;
    game->set_grid = set_grid;
    game->start = start;

    game->started = 0;
}
