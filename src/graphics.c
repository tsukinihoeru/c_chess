#include "graphics.h"

void init_colors() {
    init_color(COLOR_WHITE, 1000, 1000, 1000);
    init_color(COLOR_GREY, 160, 160, 160);
    init_pair(WHITE_PIECE_LIGHT_SQUARE_C, COLOR_WHITE, COLOR_GREY);
    init_pair(WHITE_PIECE_DARK_SQUARE_C, COLOR_WHITE, COLOR_BLACK);
    init_pair(BLACK_PIECE_LIGHT_SQUARE_C, COLOR_GREEN, COLOR_GREY);
    init_pair(BLACK_PIECE_DARK_SQUARE_C, COLOR_GREEN, COLOR_BLACK);
}

char pp_arr[16] = {'0', '1', '2', '3', 'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'};

static int get_square_color(int piece, int i, int j) {
    if (piece & 1) {
        if (i % 2 == j % 2) {
            return BLACK_PIECE_DARK_SQUARE_C;
        } else {
            return BLACK_PIECE_LIGHT_SQUARE_C;
        }
    } else {
        if (i % 2 == j % 2) {
            return WHITE_PIECE_DARK_SQUARE_C;
        } else {
            return WHITE_PIECE_LIGHT_SQUARE_C;
        }
    }
}

void draw_board(WINDOW *win, int *mailbox) {
    for (int i = 7; i >= 0; i--) {
        for(int k = 0; k < 6; k++){
            wmove(win, (7 - i) * 6 + k, 0);
            for (int j = 0; j < 8; j++) {
                int piece = mailbox[i * 8 + j];
                int color = get_square_color(piece, i, j);
                wattron(win, COLOR_PAIR(color));
                if(piece){
                    for(int ch = 0; ch < 12; ch++){
                        waddch(win, piece_art[(piece >> 1) - 2][k][ch]);
                    }
                }
                else{
                    wprintw(win, "            ");
                }
                wattroff(win, COLOR_PAIR(color));
            }
        }
    }
    wrefresh(win);
};