#include "graphics.h"
#include "../move_gen/bitboard.h"
#include <string.h>

uint16_t move_history[500];
int history_size;
int cur_move_ind; //current means the move that will be played, not the last played move
WINDOW* history_window;
const int HPAD = 100;
const int VPAD = 2;
const int WIDTH = 50;
const int HEIGHT = 25;

void print_move_history();

void init_move_history(){
    history_size = 0;
    cur_move_ind = 0;
    history_window = newwin(HEIGHT, WIDTH, VPAD, HPAD);
    box(history_window, 0, 0);
    wrefresh(history_window);
    refresh();
}

void reset_move_history(){
    history_size = 0;
    cur_move_ind = 0;
}

bool can_undo_move(){
    return cur_move_ind != 0;
}

bool can_redo_move(){
    return cur_move_ind < history_size;
}

uint16_t undo_move(){
    assert(can_undo_move());
    return move_history[--cur_move_ind];
}

uint16_t redo_move(){
    assert(can_redo_move());
    return move_history[cur_move_ind++];
}

void append_move(uint16_t move){
    move_history[cur_move_ind] = move;
    cur_move_ind++;
    history_size = cur_move_ind;
    print_move_history();
}

void print_move_history(){
    werase(history_window);
    char pgn_string[10];
    Board board;
    int line_length = 0;
    parse_board(&board, STARTING_POSITION_FEN);
    for(int i = 0; i < history_size; i++){
        get_move_pgn(&board, move_history[i], pgn_string);
        make_move(&board, move_history[i]);
        if(line_length + 1 + strnlen(pgn_string, 10) >= WIDTH){
            line_length = 1 + strnlen(pgn_string, 10);
            wprintw(history_window, "\n");
        }else{
            line_length += 1 + strnlen(pgn_string, 10);
        }
        wprintw(history_window, "%s ", pgn_string);
    }
    wrefresh(history_window);
    refresh();
}