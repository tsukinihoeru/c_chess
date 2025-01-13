#include "move_gen/bitboard.h"
#include "graphics.h"

int main() {
    
    Board board;
    char fen[] = "r3k2r/1p6/B6B/3p4/8/3K4/8/8 w kq - 0 1";
    parse_board(&board, fen);
    init_magics();
    uint16_t move_list[218];
    print_board(&board);
    int num_moves = generate_moves(&board, move_list);
    for(int i = 0; i < 5; i++){
        print_move(move_list[i]);
        printf("\n");
        make_move(&board, move_list[i]);
        print_board(&board);
        unmake_move(&board, move_list[i]);
        printf("\n");
        print_board(&board);
    }
    return 0;
}

/*
    WINDOW *board_win;
    Board board;
    MEVENT event;
    int c;
    // char str [80];
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
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
*/