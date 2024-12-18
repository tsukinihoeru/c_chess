#include "bitboard.h"
#include "graphics.h"

int main(){
    WINDOW * win;
    Board board;
    //char str [80];
    char fen[] = "rnb1k2r/1pq1bppp/5n2/pPp1p3/2Pp4/3PPNP1/PB3P1P/RN1QKB1R w KQkq - 1 9";
    parse_board(&board, fen);
    print_board(&board);
    
    for(int i = 0; i < splash_screen_art_length; i++){
        printf("%s", splash_screen_art[i]);
        printf("\n");
    }
    initscr();
    clear();
    start_color();
    init_colors();
    refresh();
    win = newwin(60, 100, 2, 2);
    draw_splash_screen(win);
    wrefresh(win);
    getch();
    werase(win);
    draw_board(win, board.mailbox);
    getch();
    endwin();
    
    return 0;
}