#include "bitboard.h"
#include "graphics.h"

int main(){
    WINDOW * win;
    Board board;
    char str [80];
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    parse_board(&board, fen);
    print_board(&board);
    initscr();
    clear();
    start_color();
    init_colors();
    refresh();
    win = newwin(8, 28, 2, 2);
    mvwprintw(win, 0, 0, "PLEASE");
    wrefresh(win);
    getch();
    draw_board(win, board.mailbox);
    getch();
    endwin();
    return 0;
}