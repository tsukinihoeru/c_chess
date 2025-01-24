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
const int HEIGHT = 40;
int BLACK_FIRST_MOVE = 0; //if black was first to move

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
}

char concat_helper[20];
void concat_str(char *s, char *t){
    char *helper = concat_helper;
    while(*t != '\0'){
        *helper = *t; helper++; t++;
    }
    char *ns = s;
    while(*ns != 0){
        *helper = *ns; helper++; ns++;
    }*helper = '\0';
    helper = concat_helper;
    while(*helper != '\0'){
        *s = *helper; helper++; s++;
    }*s = '\0';
}

void draw_move_history(){
    werase(history_window);
    char pgn_string[20];
    Board board;
    int line_length = 0;
    parse_board(&board, STARTING_POSITION_FEN);
    wprintw(history_window, "\n");
    for(int i = 0; i < history_size; i++){
        get_move_pgn(&board, move_history[i], pgn_string);
        make_move(&board, move_history[i]);
        if(i % 2 == 0){
            int move_number = i/2 + 1;
            int m_num_length = move_number / 10 + 3;
            if(line_length + strnlen(pgn_string, 10) + m_num_length >= WIDTH){
                line_length = m_num_length + strnlen(pgn_string, 10);
                wprintw(history_window, "\n");
            }
            else
                line_length += m_num_length + strnlen(pgn_string, 10);
            wprintw(history_window, " %d.", move_number);
            if(cur_move_ind - 1 == i)
                wattron(history_window, A_UNDERLINE);
            wprintw(history_window, "%s", pgn_string);
            wattroff(history_window, A_UNDERLINE);
        }
        else{
            if(line_length + strnlen(pgn_string, 10) >= WIDTH){
                line_length = 1 + strnlen(pgn_string, 10);
                wprintw(history_window, "\n");
            }else{
                line_length += 1 + strnlen(pgn_string, 10);
            }
            wprintw(history_window, " ");
            if(cur_move_ind - 1 == i)
                wattron(history_window, A_UNDERLINE);
            wprintw(history_window, "%s", pgn_string);
            wattroff(history_window, A_UNDERLINE);
        }
    }
    box(history_window, 0, 0);
    wrefresh(history_window);
    refresh();
}