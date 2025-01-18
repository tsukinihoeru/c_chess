#include <ncurses.h>
#include "graphics.h"

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

void init_colors() {
    start_color();
    init_color(COLOR_GREY, 160, 160, 160);
    init_color(COLOR_DARK_RED, 240, 120, 120);
    init_pair(WHITE_PIECE_LIGHT_SQUARE_C, WHITE_PIECE_COLOR, LIGHT_SQUARE_COLOR);
    init_pair(WHITE_PIECE_DARK_SQUARE_C, WHITE_PIECE_COLOR, DARK_SQUARE_COLOR);
    init_pair(WHITE_PIECE_HIGHLIGHT_SQUARE_C, WHITE_PIECE_COLOR, HIGHLIGHT_SQUARE_COLOR);
    init_pair(BLACK_PIECE_LIGHT_SQUARE_C, BLACK_PIECE_COLOR, LIGHT_SQUARE_COLOR);
    init_pair(BLACK_PIECE_DARK_SQUARE_C, BLACK_PIECE_COLOR, DARK_SQUARE_COLOR);
    init_pair(BLACK_PIECE_HIGHLIGHT_SQUARE_C, BLACK_PIECE_COLOR, HIGHLIGHT_SQUARE_COLOR);
}

int get_square_color(int piece, int i, int j) {
    if (piece & 1) {
        if(square_highlighted(i * 8 + j))
            return BLACK_PIECE_HIGHLIGHT_SQUARE_C;
        else if (i % 2 == j % 2) {
            return BLACK_PIECE_DARK_SQUARE_C;
        } else {
            return BLACK_PIECE_LIGHT_SQUARE_C;
        }
    } else {
        if(square_highlighted(i * 8 + j))
            return WHITE_PIECE_HIGHLIGHT_SQUARE_C;
        else if (i % 2 == j % 2) {
            return WHITE_PIECE_DARK_SQUARE_C;
        } else {
            return WHITE_PIECE_LIGHT_SQUARE_C;
        }
    }
}