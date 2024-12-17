#include <ncurses.h>
#include <wchar.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

static const short COLOR_GREY = 14;

static const short BLACK_PIECE_DARK_SQUARE_C = 2;
static const short BLACK_PIECE_LIGHT_SQUARE_C = 3;
static const short WHITE_PIECE_DARK_SQUARE_C = 4;
static const short WHITE_PIECE_LIGHT_SQUARE_C = 5;

static const int SQUARE_SIZE_VERT = 4;

void init_colors();
void draw_board(WINDOW *win, int *mailbox);

static const char piece_art[6][6][15] = {
    {
        "             ",
        "     _       ",
        "    (_)      ",
        "   (___)     ",
        "   _|_|_     ",
        "  (_____)    "
    },{
        "    |\\_      ",
        "   /  .\\_    ",
        "  |   ___)   ",
        "   \\ = |     ",
        "   /____\\    ",
        "  [______]   "
    },{
        "     _0      ",
        "   / //\\     ",
        "  {     }    ",
        "   \\   /     ",
        "   _|_|_     ",
        "  (_____)    "
    },{
        "  __  _  _   ",
        " |________|  ",
        " \\__ ____ /  ",
        "  |____|_|   ",
        "  |__|___|   ",
        " [________]  "
    },{
        "    _()_     ",
        "  _/____\\_   ",
        "  \\______/   ",
        "   (____)    ",
        "    |__|     ",
        "  (______)   "
    },{
        "    _++_     ",
        "  _/____\\_   ",
        "  \\______/   ",
        "   (____)    ",
        "    |__|     ",
        "  (______)   "
    }
};

#endif