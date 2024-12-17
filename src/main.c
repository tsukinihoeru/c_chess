#include "bitboard.h"
#include "graphics.h"

void draw_b(int *mailbox) {
    for (int i = 7; i >= 0; i--) {
        for(int k = 0; k < 6; k++){
            for (int j = 0; j < 8; j++) {
                int piece = mailbox[i * 8 + j];
                if(piece){
                    for(int ch = 0; ch < 12; ch++){
                        printf("%c", piece_art[(piece >> 1) - 2][k][ch]);
                    }
                }
                else{
                    printf("            ");
                }
            }printf("\n");
        } 
    }
};

int main(){
    WINDOW * win;
    Board board;
    //char str [80];
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    parse_board(&board, fen);
    print_board(&board);
    
    initscr();
    clear();
    start_color();
    init_colors();
    refresh();
    win = newwin(60, 100, 2, 2);
    mvwprintw(win, 0, 0, "PLEASE");
    wrefresh(win);
    getch();
    draw_board(win, board.mailbox);
    getch();
    endwin();
    
    return 0;
}