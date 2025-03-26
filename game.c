#pragma once

#include "game.h"
#include "utils.h"
#include "interface.h"
#include "screen.h"

#include "math.h"

#define DEFAULT_BOUNDS (_bounds){0, 0}

_game* game;
wchar_t* difficulties[3] = { L"Easy", L"Normal", L"Hard" };
_screen* get_screen_instance(_game* game_ref);
_menu* get_menu_instance(_bounds bounds);

int solved_board[9][9];

int board[9][9] = { 0 };

int removed = 0;
int replaced = 0;

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

void populate_board(int* array, int position) {
    int x_bound = ((position - 1) % 3 + 1) * 3;
    int y_bound = ((position - 1) / (game->grid / 3) + 1) * (game->grid / 3);
    int i = 0;

    for (int k = y_bound - (game->grid / 3); k < y_bound; k++) {
        for (int l = x_bound - 3; l < x_bound; l++) {
            solved_board[l][k] = array[i];
            if (i >= game->grid - 1) i = 0;
            i++;
        }
    }
}

int is_safe(int t_board[9][9], int column, int row, int n) {
    // check vertical
    for (int y = 0; y < game->grid; y++) {
        if (t_board[column][y] == n)
            return 0;
    }

    //check horizontal
    for (int x = 0; x < game->grid; x++) {
        if (t_board[x][row] == n)
            return 0;
    }

    //check square
    int x_bound = ((column / 3) + 1) * 3;
    int y_bound = ((row / (game->grid / 3)) + 1) * (game->grid / 3);

    for (int i = y_bound - (game->grid / 3); i < y_bound; i++) {
        for (int j = x_bound - 3; j < x_bound; j++) {
            if (n == t_board[j][i])
                return 0;
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
        if (is_safe(solved_board, row, column, num)) {
            solved_board[row][column] = num;
            if (solve_sudoku(row, column + 1)) {
                return 1;
            }
        }
        solved_board[row][column] = 0;
    }

    return 0;
}

int is_unique(int row, int column) {
    int count = 0;
    for (int num = 1; num <= game->grid; num++) {
        if (is_safe(board, row, column, num)) count++;
        if (count > 1) break;
    }
    return (count == 1) ? 1 : 0;
}

void remove_numbers(int numbers_to_remove) {
    int count = 0;
    for (int i = 0; i < numbers_to_remove; i++) {
        int row = rand() % game->grid;
        int column = rand() % game->grid;

        int temp = board[row][column];
        board[row][column] = 0;

        int unique = is_unique(row, column);

        if (unique != 1) {
            board[row][column] = temp;
        }
        
        if (board[row][column] != temp) count++;
    }
    removed = count;
}

void copy_board() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = solved_board[i][j];
        }
    }
}

void generate_board() {
    // pool of numbers to choose from
    int numbers_stack[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    memset(solved_board, 0, sizeof(solved_board));

    // populating the board in squares that aren't connected vertically or horizontally with each other
    int spacing = (game->grid % 2 == 0) ? 2 : 3;
    for (int i = 1; i <= game->grid; i += ((game->grid/spacing) + 1)) {
        // shuffle takes into an account the size of grids, so if its 6, only the first 6 numbers in the array will be shuffled.
        shuffle(numbers_stack, game->grid);
        // populate board (array, int) where the int corresponds to a square inside the full grid. grid[9x9] is composed of 9 squares sized 3x3
        // is later used for backtracking
        populate_board(numbers_stack, i);
    }

    // fill the rest of the board using backtracking - filling all zeroes according to sudoku rules.
    solve_sudoku(0, 0);

    //copy the solved board to the actual board that'll be shown on screen
    copy_board();

    // will attempt to remove X numbers.
    // but will remove less if the solution isn't unique.
    remove_numbers(min(game->difficulty * game->grid * 3, game->grid * game->grid));
}

void draw_board_numbers(int highlight) {
    _screen* screen = get_screen_instance(NULL);

    // Centering the grid
    int board_size_y = game->grid + (int)(game->grid / 3);
    int board_size_x = game->grid * 2 + (int)(game->grid / 3) * 2;
    int y_offset = (Utils.get_console_height() / 2 - board_size_y / 2) + 1;
    int x_offset = (Utils.get_console_width() / 2 - board_size_x / 2) + 2;

    // Depening on the grid size, board needs different spacings between the numbers
    // 9x9 is 9 * 3x3 grids
    // 6x6 is 6 * 2x3 grids
    // 3x3 is 3 * 1x3 grids
    // could factor those if i want to create custom grids such as (4x4, 15x15, etc.), but there are also prime numbers (eg. 17) and i don't wanna deal with that.
    // gonna stick with just 9x9, 6x6 and 3x3.

    int spacing = (game->grid % 2 == 0) ? 2 : 3; // if the grid size is even (6), make the spacing 2, otherwise 3
    int row_spacing = 0;
    int column_spacing = 0;
    int count = 0;
    for (int column = 0; column < game->grid; column++) {
        for (int row = 0; row < game->grid; row++) {
            int n = board[column][row];
            short x = x_offset + column + column_spacing;
            short y = y_offset + row + row_spacing;
            COORD pos = (COORD){ x, y };
            wchar_t* underscore = screen->get_draw_empty() ? L"_" : L"\033[4m ";

            if (n != 0 && highlight == -1 || n != 0 && count == screen->menu->previous_position) { // draw the number
                Interface.draw_text(pos, L"%x", n);
                if (n != solved_board[column][row]) // incorrect number
                    Interface.draw_text(pos, L"\033[41m%x", n);
            }

            if (n == 0) Interface.draw_text(pos, L"\033[90m%s", screen->get_draw_empty() ? L"_" : L" "); // draw underscore

            if (count == highlight || (highlight + replaced < 0 && count == 0)) { // highlight selected cells
                Interface.draw_text(pos, L"\033[9%dm%s%x", screen->get_theme(), screen->get_draw_empty() ? L"" : L"\033[4m", n);
                if(n != solved_board[column][row]) Interface.draw_text(pos, L"\033[101m%x", n); // highlight incorrect number
                if (n == 0) Interface.draw_text(pos, L"\033[9%dm%s", screen->get_theme(), screen->get_draw_empty() ? L"_" : L"\033[4m "); // highlight underscore
            }

            if ((row + 1) % spacing == 0) row_spacing++; // add additional row spacing after each (nd|rd) column
            count++;
        }
        row_spacing = 0; // revert it to 0 every column
        if ((column + 1) % 3 == 0) column_spacing += 2; // add additional spacing after each third column
        column_spacing++; // add additional spacing after each column
    }
}

void draw_board_grid() {
    _screen* screen = get_screen_instance(NULL);

    int spacing = (game->grid % 2 == 0) ? 2 : 3; // if the grid size is even (6), make the spacing 2, otherwise 3

    // Centering the grid
    int board_size_y = game->grid + (int)(game->grid / 3) + (3 - spacing);
    int board_size_x = game->grid * 2 + (int)(game->grid / 3) * 2;
    int y_offset = Utils.get_console_height() / 2 - board_size_y / 2;
    int x_offset = Utils.get_console_width() / 2 - board_size_x / 2;

    int previous_row_spacing = 0;
    int column_spacing = 0;
    for (int column = 0; column <= board_size_x; column++) {
        for (int row = 0; row <= board_size_y; row++) {
            if (column % 8 == 0) Interface.draw_text((COORD) { x_offset + column, y_offset + row }, L"│");

            if (row % (spacing + 1) == 0) {
                Interface.draw_text((COORD) { x_offset + column, y_offset + row }, L"─");
                if (column % 8 == 0) Interface.draw_text((COORD) { x_offset + column, y_offset + row }, L"┼");
                if (column == 0) Interface.draw_text((COORD) { x_offset + column, y_offset + row }, L"├");
                if (column == board_size_x) Interface.draw_text((COORD) { x_offset + column, y_offset + row }, L"┤");
            }
        }

        if (column % 8 == 0) {
            Interface.draw_text((COORD) { x_offset + column, y_offset }, L"┬"); // Between number grids at the top
            Interface.draw_text((COORD) { x_offset + column, y_offset + board_size_y }, L"┴"); // Between number grids at the bottom
        }

        if (column == 0) {
            Interface.draw_text((COORD) { x_offset + column, y_offset }, L"┌"); // Top left corner
            Interface.draw_text((COORD) { x_offset + column, y_offset + board_size_y }, L"└"); // Bottom-left corner
        }

        if (column == board_size_x) {
            Interface.draw_text((COORD) { x_offset + column, y_offset }, L"┐"); // Top-right corner
            Interface.draw_text((COORD) { x_offset + column, y_offset + board_size_y }, L"┘"); // Bottom-right corner
        }

        if ((column + 1) % 3 == 0) column_spacing += 2;
        column_spacing++;
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

void g_update_impl(int from_selection) {
    _menu* menu = get_menu_instance(DEFAULT_BOUNDS);
    int highlight = (from_selection != -1) ? menu->position : -1;
    draw_board_numbers(highlight);
    draw_board_grid();
}

void game_ui() {
    _menu* menu = get_menu_instance(DEFAULT_BOUNDS);
    _screen* screen = get_screen_instance(NULL);
    // They're 'dynamically' updated in the menu.c file, because like, why not.
    wchar_t buffer[128];
    const wchar_t* labels[] = { L"ESC_QUIT", L"NO_PLACE", L"UP_DOWN", L"RIGHT_LEFT"/*, L"ELAPSED" overflows the drawing buffer every second*/, L"MISTAKES"};
    
    for (int i = 0; i < sizeof(labels) / sizeof(labels[0]); i++) {
        Utils.write_literal(buffer, sizeof(buffer), labels[i]);
        menu->update_label(&menu->labels[i], buffer, (COORD) { (short)NULL, (short)NULL });
    }
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
    }
}

int select(int n) {
    _menu* menu = get_menu_instance(DEFAULT_BOUNDS);
    int p = menu->position;

    // already finished the game dont click anything please
    if (replaced == -1) return EXIT_SUCCESS;

    if (n >= 0xFFF) {
        int number = n - 0xFFF;
        if (number > game->grid) return EXIT_FAILURE;
        int row = menu->position / game->grid;
        int column = menu->position % game->grid;

        if (board[row][column] != solved_board[row][column]) {
            board[row][column] = number;
            // Check if it's a wrong number
            if (number != solved_board[row][column] && number != 0) {
                game->mistakes++;
                menu->update(); // updates the labels
            }
            else if(number == solved_board[row][column] && number != 0) replaced++;
            draw_board_numbers(menu->position);
        }

        // Solved the sudoku board
        if (replaced == removed) {
            menu->remove_labels();
            
            system("cls");

            game->total_elapsed = game->elapsed;
            game->score = max(0, 1000 - (10 * game->mistakes) - (1.5 * game->elapsed));
            

            wchar_t buffer[128];
            const wchar_t* labels[] = { L"ESC_QUIT", L"CONGRATULATIONS", L"STAT_TIME", L"STAT_MISS", L"STAT_DIFF", L"SCORE"};
            for (int i = 0; i < sizeof(labels) / sizeof(labels[0]); i++) {
                Utils.write_literal(buffer, sizeof(buffer), labels[i]);
                menu->update_label(&menu->labels[i], buffer, (COORD) { (short)NULL, (short)NULL });
            }

            menu->position = -1;

            g_update_impl(-1);

            replaced = -1;
        }

        return EXIT_SUCCESS;
    }

    if (n < 0) {
        return EXIT_FAILURE;
    }

    int direction = n - p;
    if (n - p == 0xA) direction = 1;
    if (n - p == 0xF) direction = -1;

    if (n - p > 1) {
        if (p + (direction * game->grid) < 0 || p + (direction * game->grid) >= game->grid * game->grid) return EXIT_FAILURE;
        menu->position += direction * game->grid;
    }
    else {
        if (n % game->grid == 0 && n - p > 0) {
            return EXIT_FAILURE;
        }

        if ((n + 1) % game->grid == 0 && n - p < 0) return EXIT_FAILURE;

        menu->position = n;
    }

    menu->previous_position = p;

    draw_board_numbers(menu->position);
    return EXIT_SUCCESS;
}

void timer_t() {
    _menu* menu = get_menu_instance(DEFAULT_BOUNDS);
    while (1 == 1) {
        Sleep(1000);
        if (game->reset_timer == 1) game->elapsed = 0;
        game->elapsed++;
    }
}

void start() {
    game->started = 1;
    game->mistakes = 0;
    game->elapsed = 0;
    replaced = 0;

    time_t start, end;

    generate_board();

    time(&start);

    _screen* screen = get_screen_instance(NULL);

    game_ui();
    g_update_impl(-1);
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
    game->timer_t = timer_t;

    game->select = select;

    game->started = 0;
}
