#include <ncurses.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

static const short BLACK_PIECE_DARK_SQUARE_C = 2;
static const short BLACK_PIECE_LIGHT_SQUARE_C = 3;
static const short WHITE_PIECE_DARK_SQUARE_C = 4;
static const short WHITE_PIECE_LIGHT_SQUARE_C = 5;

void init_colors();
void draw_board(WINDOW * win, int * mailbox);

#endif