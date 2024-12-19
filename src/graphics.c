#include "graphics.h"

int highlighted_square = -1;

void set_highlighted_square(int square){
    highlighted_square = square;
}

void init_graphics(){
    initscr();
    clear();
    noecho();
    cbreak();
    init_colors();
    keypad(stdscr, 1);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    refresh();
}

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

char pp_arr[16] = {'0', '1', '2', '3', 'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'};

static int get_square_color(int piece, int i, int j) {
    if (piece & 1) {
        if(i * 8 + j == highlighted_square)
            return BLACK_PIECE_HIGHLIGHT_SQUARE_C;
        else if (i % 2 == j % 2) {
            return BLACK_PIECE_DARK_SQUARE_C;
        } else {
            return BLACK_PIECE_LIGHT_SQUARE_C;
        }
    } else {
        if(i * 8 + j == highlighted_square)
            return WHITE_PIECE_HIGHLIGHT_SQUARE_C;
        else if (i % 2 == j % 2) {
            return WHITE_PIECE_DARK_SQUARE_C;
        } else {
            return WHITE_PIECE_LIGHT_SQUARE_C;
        }
    }
}

void draw_board(WINDOW *win, int *mailbox) {
    mvprintw(51, 0, "Square: %d", highlighted_square);
    for (int i = 7; i >= 0; i--) {
        for(int k = 0; k < 6; k++){
            wmove(win, (7 - i) * 6 + k, 0);
            for (int j = 0; j < 8; j++) {
                int piece = mailbox[i * 8 + j];
                int color = get_square_color(piece, i, j);
                wattron(win, COLOR_PAIR(color));
                if(piece){
                    wprintw(win, "%s", piece_art[(piece >> 1) - 2][k]);
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

static char splash_message[] = ">Press Any Key to Continue<";

void draw_splash_screen(WINDOW * win){
    for(int i = 0; i < splash_screen_art_length; i++){
        mvwprintw(win, i, splash_screen_hpad, "%s", splash_screen_art[i]);
    }
    struct timespec remaining, request = {0, 50000000L}; ;
    wmove(win, 20, splash_screen_hpad + 14);
    for(int i = 0; i < 27; i++){
        nanosleep(&request, &remaining);
        waddch(win, splash_message[i]);
        wrefresh(win);
    }
}

bool point_in_window(WINDOW *win, int px, int py){
    int beg_x, beg_y;
    int size_x, size_y;
    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, size_y, size_x);
    return px >= beg_x && px < beg_x + size_x && py >= beg_y && py < beg_y + size_y;
}

int cursor_to_square_index(int cposx, int cposy){
    int file = (cposx - BOARD_HPAD)/SQUARE_WIDTH;
    int rank = 7 - (cposy - BOARD_VPAD)/SQUARE_HEIGHT;
    if(file > 7 || file < 0 || rank > 7 || rank < 0)
        return -1;
    int square = rank * 8 + file;
    return square;
}