#include <ncurses.h>
#include <wchar.h>
#include <time.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

static const short COLOR_GREY = 14;
static const short COLOR_DARK_RED = 15;
static const short WHITE_PIECE_COLOR = COLOR_WHITE;
static const short BLACK_PIECE_COLOR = COLOR_GREEN;
static const short HIGHLIGHT_SQUARE_COLOR = COLOR_DARK_RED;
static const short DARK_SQUARE_COLOR = COLOR_BLACK;
static const short LIGHT_SQUARE_COLOR = COLOR_GREY;


static const short BLACK_PIECE_DARK_SQUARE_C = 2;
static const short BLACK_PIECE_LIGHT_SQUARE_C = 3;
static const short WHITE_PIECE_DARK_SQUARE_C = 4;
static const short WHITE_PIECE_LIGHT_SQUARE_C = 5;
static const short BLACK_PIECE_HIGHLIGHT_SQUARE_C = 6;
static const short WHITE_PIECE_HIGHLIGHT_SQUARE_C = 7;


static const int SQUARE_HEIGHT = 6;
static const int SQUARE_WIDTH = 12;
static const int BOARD_HPAD = 2;
static const int BOARD_VPAD = 2;

void init_graphics();
void init_colors();
void draw_board(WINDOW *win, int *mailbox);
bool point_in_window(WINDOW *win, int px, int py);
int cursor_to_square_index(int cposx, int cposy);
void set_highlighted_square(int square);

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