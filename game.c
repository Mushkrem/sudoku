#pragma once

#include "game.h"
#include "utils.h"
#include "interface.h"
#include "screen.h"

#define DEFAULT_BOUNDS (_bounds){0, 0}

_game* game;
wchar_t* difficulties[3] = { L"Easy", L"Normal", L"Hard" };
_screen* get_screen_instance(_game* game_ref);
_menu* get_menu_instance(_bounds bounds);

int dummy_solved_board[9][9] = {        //  ┌─────────┬─────────┬─────────┐
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

int solved_board[9][9];

int board[9][9] = { 0 };

void shuffle(int* array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        if (array[j] <= game->grid) {
            int t = array[i];
            array[i] = array[j];
            array[j] = t;
        }
    }
}

void shuffle_two(int* array) {
    for (int i = 0; i < game->grid - 1; i++) {

    }
}

void populate_board(int* array, int position) {
    int x_bound = ((position - 1) % 3 + 1) * 3;
    int y_bound = ((position - 1) / 3 + 1) * 3;
    int i = 0;

    for (int k = y_bound - 3; k < y_bound; k++) {
        for (int l = x_bound - 3; l < x_bound; l++) {
            solved_board[l][k] = array[i];
            if (i >= game->grid - 1) i = 0;
            i++;
        }
    }
}

int is_safe(int column, int row, int n) {
    // check vertical
    for (int y = 0; y < game->grid; y++) {
        if (solved_board[column][y] == n)
            return 0;
    }

    //check horizontal
    for (int x = 0; x < game->grid; x++) {
        if (solved_board[x][row] == n)
            return 0;
    }

    //check square
    int x_bound = ((column / 3) + 1) * 3;
    int y_bound = ((row / 3) + 1) * 3;

    for (int i = y_bound - 3; i < y_bound; i++) {
        for (int j = x_bound - 3; j < x_bound; j++) {
            if (n == solved_board[j][i])
                return 1;
        }
    }

    return 1;
}

int solve_sudoku(int row, int column) {
    if (row == game->grid - 1 && column == game->grid)
        return 1;

    if (column == game->grid) {
        row++;
        column = 0;
    }

    if (solved_board[row][column] > 0) {
        return solve_sudoku(row, column + 1);
    }

    for (int num = 1; num <= game->grid; num++) {
        if (is_safe(row, column, num)) {
            //Utils.debug(L"Y, Column = %d, Row = %d, No = %d\n", column, row, num);
            solved_board[row][column] = num;
            if (solve_sudoku(row, column + 1)) {
                return 1;
            }
        }
        solved_board[row][column] = 0;
    }

    return 0;
}

void generate_board() {
    for (int i = 0; i < 9; i++) { 
        for (int j = 0; j < 9; j++) { 
            solved_board[i][j] = 0; 
        } 
    }

    // 9x9 generation:
    // first generate one 3x3 square
    int numbers_stack[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    srand(time(NULL) ^ getpid() << 16); // time(NULL) alone wasn't really random

    shuffle(numbers_stack, 9);

    wchar_t buffer[128];
    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < 9; i++) {
        if (i != 0) {
            Utils.write_literal(buffer, sizeof(buffer), L"%s %d ", buffer, numbers_stack[i]);
        }
        else if (i == 0) {
            Utils.write_literal(buffer, sizeof(buffer), L" %d ", numbers_stack[i]);
        }
    }
    Utils.debug(buffer);

    populate_board(numbers_stack, 1);

    solve_sudoku(0, 0);
}

void draw_odd_board() { // i dont care i cant d
    _screen* screen = get_screen_instance(NULL);
    int board_size_y = game->grid + (int)(game->grid / 3);
    int board_size_x = game->grid * 2 + (int)(game->grid / 3) * 2;
    int y_offset = Utils.get_console_height() / 2 - board_size_y / 2;
    int x_offset = Utils.get_console_width() / 2 - board_size_x / 2;
    int x_inc = 0; // additional spacing for vertical borders between the numbers
    int y_inc = 0; // additional spacing for horizontal borders between the numbers

    for (int column = 0; column <= board_size_x; column++) { // <= so the border is bigger
        for (int row = 0; row <= board_size_y; row++) {
            
            if (row < game->grid && column < game->grid) { // Inner numbers
                int n = solved_board[column][row];
                if (n != 0) {
                    Interface.draw_text((COORD) { x_offset + 2 + x_inc * 2, y_offset + 1 + y_inc }, L"%x", n);
                }
                else {
                    Interface.draw_text((COORD) { x_offset + 2 + x_inc * 2, y_offset + 1 + y_inc }, L"┄");
                }
                if ((column + 1) % 3 == 0)
                    Interface.draw_text((COORD) { x_offset + 2 + x_inc * 2 + 2, y_offset + 1 + y_inc }, L"│");

                if (column == 0)
                    Interface.draw_text((COORD) { x_offset - 2 + x_inc * 2 + 2, y_offset + 1 + y_inc }, L"│");
            }

            if (row == 0) {
                Interface.draw_text((COORD) { column + x_offset, y_offset }, L"─");
                if (column == 0) Interface.draw_text((COORD) { x_offset, y_offset }, L"┌");
                if (column == board_size_x) Interface.draw_text((COORD) { board_size_x + x_offset, y_offset }, L"┐");
                if (column % 6 == 0 && column != 0) {
                    Interface.draw_text((COORD) { x_offset + x_inc, y_offset }, L"┬");
                }
            }

            y_inc++;
            if ((row + 1) % 2 == 0) y_inc++;
        }
        y_inc = 0;
        x_inc++;

        if ((column + 1) % 3 == 0) x_inc++;
    }
}

void draw_board() {
    _screen* screen = get_screen_instance(NULL);
    int board_size_y = game->grid + (int)(game->grid / 3);
    int board_size_x = game->grid * 2 + (int)(game->grid / 3)*2;
    int y_offset = Utils.get_console_height() / 2 - board_size_y / 2;
    int x_offset = Utils.get_console_width() / 2 - board_size_x / 2;
    int x_inc = 0; // additional spacing for vertical borders between the numbers
    int y_inc = 0; // additional spacing for horizontal borders between the numbers

    int is_odd = (game->grid % 2 == 0) ? 0 : 1;

    if (!is_odd) {
        draw_odd_board(); // i dont care this is stupid
        return; // i hate this
    }

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

void g_update_impl() {
    draw_board();
}

void tutorial() {
    _menu* menu = get_menu_instance(DEFAULT_BOUNDS);
    _screen* screen = get_screen_instance(NULL);

    wchar_t buffer[128];
    Utils.write_literal(buffer, sizeof(buffer), L"ESC_QUIT");
    menu->update_label(&menu->labels[0], buffer, (COORD) { NULL, NULL });

    Utils.write_literal(buffer, sizeof(buffer), L"NO_PLACE");
    menu->update_label(&menu->labels[1], buffer, (COORD) { NULL, NULL });

    Utils.write_literal(buffer, sizeof(buffer), L"UP_DOWN");
    menu->update_label(&menu->labels[2], buffer, (COORD) { NULL, NULL });

    Utils.write_literal(buffer, sizeof(buffer), L"RIGHT_LEFT");
    menu->update_label(&menu->labels[3], buffer, (COORD) { NULL, NULL });

}

void print_board() {
    // temporary
    wchar_t buffer[128];
    for (int i = 0; i < 9; i++) {
        memset(buffer, 0, sizeof(buffer));
        for (int j = 0; j < 9; j++) {
            if (j != 0) {
                Utils.write_literal(buffer, sizeof(buffer), L"%s %d ", buffer, solved_board[j][i]);
            }
            else if (j == 0) {
                Utils.write_literal(buffer, sizeof(buffer), L" %d ", solved_board[j][i]);
            }
        }
        Utils.debug(buffer);
    }
}


void start() {
    game->started = 1;

    generate_board();

    print_board();

    tutorial();
    g_update_impl();
}

void stop() {
    game->started = 0;
}

void g_init(_game* g) {
	game = g;
	
	game->get_difficulty = get_difficulty;
    game->set_difficulty = set_difficulty;
    game->update = g_update_impl;
	game->get_grid = get_grid;
    game->set_grid = set_grid;
    game->start = start;
    game->stop = stop;

    game->started = 0;
}
