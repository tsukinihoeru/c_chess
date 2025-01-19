#include "graphics.h"

uint16_t move_history[500];
int history_size;
int cur_move_ind; //current means the move that will be played, not the last played move

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
}