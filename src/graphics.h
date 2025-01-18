#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
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

//test functions (delete later)
void exit_curses_get_fen_input(char *str, int lim);

static const char piece_art[6][SQUARE_HEIGHT][SQUARE_WIDTH + 1] = {
    {
        "            ",
        "     _      ",
        "    (_)     ",
        "   (___)    ",
        "   _|_|_    ",
        "  (_____)   "
    },{
        "    |\\_     ",
        "   /  .\\_   ",
        "  |   ___)  ",
        "   \\ = |    ",
        "   /____\\   ",
        "  [______]  "
    },{
        "     _0     ",
        "   / //\\    ",
        "  {     }   ",
        "   \\   /    ",
        "   _|_|_    ",
        "  (_____)   "
    },{
        "  __  _  _  ",
        " |________| ",
        " \\__ ____ / ",
        "  |____|_|  ",
        "  |__|___|  ",
        " [________] "
    },{
        "    _()_    ",
        "  _/____\\_  ",
        "  \\______/  ",
        "   (____)   ",
        "    |__|    ",
        "  (______)  "
    },{
        "    _++_    ",
        "  _/____\\_  ",
        "  \\______/  ",
        "   (____)   ",
        "    |__|    ",
        "  (______)  "
    }
};

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