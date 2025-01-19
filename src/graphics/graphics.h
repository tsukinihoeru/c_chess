#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

static const int SQUARE_HEIGHT = 6;
static const int SQUARE_WIDTH = 12;
static const int BOARD_HPAD = 2;
static const int BOARD_VPAD = 2;

//graphics.c functions
void init_graphics();
void init_colors();

bool point_in_window(WINDOW *win, int px, int py);
int cursor_to_window_x(int cposx);
int cursor_to_window_y(int cposy);
void set_highlighted_square(int square);

//board_window functions
void init_board_win(WINDOW *win);
void receive_input(int win_x, int win_y);
void draw_board();
bool square_highlighted(int square);

//move_history functions
void reset_move_history();
bool can_undo_move();
bool can_redo_move();
uint16_t undo_move();
uint16_t redo_move();
void append_move(uint16_t move);


//test functions (delete later)
void exit_curses_get_fen_input(char *str, int lim);

void draw_splash_screen(WINDOW * win);

static const int splash_screen_hpad = 20;

static const char splash_screen_art_length = 17;
static const char splash_screen_art[17][55] = {
    "      # ###      /                                    ",
    "    /  /###  / #/                                     ",
    "   /  /  ###/  ##                                     ",
    "  /  ##   ##   ##                                     ",
    " /  ###        ##                                     ",
    "##   ##        ##  /##      /##       /###     /###   ",
    "##   ##        ## / ###    / ###     / #### / / #### /",
    "##   ##        ##/   ###  /   ###   ##  ###/ ##  ###/ ",
    "##   ##        ##     ## ##    ### ####     ####      ",
    "##   ##        ##     ## ########    ###      ###     ",
    "##  ##         ##     ## #######       ###      ###   ",
    " ## #       /  ##     ## ##              ###      ### ",    
    "  ###      /   ##     ## ####    /  /###  ## /###  ## ",
    "   ######/     ##     ##  ######/  / #### / / #### /  ",
    "     ###       ###    ##   #####      ###/     ###/   ",
    "                     /                                ",
    "                    /                                 "
};

#endif