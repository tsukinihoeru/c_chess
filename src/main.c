#include "move_gen/bitboard.h"
#include "graphics/graphics.h"
#include <time.h> 
char str [80];

int main() {
    
    WINDOW *board_win = NULL;
    MEVENT event;
    int c;
    // char str [80];
    init_graphics();
    init_board_win(board_win);
    draw_board();
    while (1) {
        //refresh();
        c = getch();
        if (c == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    receive_input(cursor_to_window_x(event.x), cursor_to_window_y(event.y));
                    draw_board();
                    //set_highlighted_square(cursor_to_square_index(event.x, event.y));
                    //draw_board(board_win, board.mailbox);
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
        }else if( c == 'p'){
            exit_curses_get_fen_input(str, 80);
            mvprintw(51, 0, "%s", str);
            refresh();
        }
        else{
            mvprintw(51, 0, "Getch %d   %d", c, KEY_MOUSE);
        }
    }
    endwin();
    return 0;
}

/*
    Board board;
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    parse_board(&board, fen);
    init_magics();
    clock_t begin = clock();
    uint64_t nodes = perft(&board, 6);
    clock_t end = clock();
    double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("%llu\n", nodes);
    printf("Finished in %f", time_spent);
    return 0;
    
*/