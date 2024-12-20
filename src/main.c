#include "bitboard.h"
#include "graphics.h"

int main() {
    WINDOW *board_win;
    Board board;
    MEVENT event;
    int c;
    // char str [80];
    char fen[] = "rnb1k2r/1pq1bppp/5n2/pPp1p3/2Pp4/3PPNP1/PB3P1P/RN1QKB1R w KQkq - 1 9";
    parse_board(&board, fen);
    init_graphics();
    
    
    board_win = newwin(8 * SQUARE_HEIGHT, 8 * SQUARE_WIDTH, BOARD_VPAD, BOARD_HPAD);
    draw_splash_screen(board_win);
    wrefresh(board_win);
    getch();
    werase(board_win);
    wrefresh(board_win);
    
    draw_board(board_win, board.mailbox);
    printw("fuck my life");
    refresh();
    while (1) {
        refresh();
        c = getch();
        if (c == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    set_highlighted_square(cursor_to_square_index(event.x, event.y));
                    draw_board(board_win, board.mailbox);
                }
            }
        }else if(c == 'q'){
            mvprintw(51, 0, "Bitch");
        } 
        else if(c == 'w'){
            mvprintw(51, 0, "Witch");
        }
        else if (c == 10 || c == 'a') {
            break;
        }else{
            mvprintw(51, 0, "Getch %d   %d", c, KEY_MOUSE);
        }
    }
    endwin();

    return 0;
}