#include "graphics.h"
#include "../move_gen/bitboard.h"
#include <string.h>
#include <string>
#include <vector>
#include <math.h>

const int MAX_PGN_LENGTH = 2000;

uint16_t move_history[500];
int history_size;
int cur_move_ind; //current means the move that will be played, not the last played move
WINDOW* history_window;
const int HPAD = 100;
const int VPAD = 1;
const int WIDTH = 50;
const int HEIGHT = 40;
int BLACK_FIRST_MOVE = 0; //if black was first to move

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

int num_digits(int num){
    int count = 0;
    while(num){
        num /= 10;
        count++;
    }return count;
}


//TODO: change so it uses std::string instead of c string
void d_move_history(){
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
            int m_num_length = num_digits(move_number) + 2;
            if(line_length + strnlen(pgn_string, 10) + m_num_length>= WIDTH){
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

void draw_move_history(){
    werase(history_window);
    char pgn_string[20];
    Board board;
    parse_board(&board, STARTING_POSITION_FEN);
    std::vector<std::string> str_list;
    str_list.push_back("");
    int cur_move_row = 0;
    int cur_move_start = 0;
    int cur_move_end = 0;
    wprintw(history_window, "\n");
    for(int i = 0; i < history_size; i++){
        get_move_pgn(&board, move_history[i], pgn_string);
        make_move(&board, move_history[i]);
        if(i % 2 == 0){
            int move_number = i/2 + 1;
            int m_num_length = num_digits(move_number) + 2;
            if(str_list.back().size() + strnlen(pgn_string, 10) + m_num_length>= WIDTH){
                str_list.push_back("");
            }str_list.back() += " " + std::to_string(move_number) + ".";
            if(i == cur_move_ind - 1){
                cur_move_row = str_list.size() - 1;
                cur_move_start = str_list.back().size();
            }
            str_list.back().append(pgn_string);
            if(i == cur_move_ind - 1){
                cur_move_end = str_list.back().size();
            }
        }else{
             if(str_list.back().size() + strnlen(pgn_string, 10) >= WIDTH){
                str_list.push_back("");
             }str_list.back() += " ";
             if(i == cur_move_ind - 1){
                cur_move_row = str_list.size() - 1;
                cur_move_start = str_list.back().size();
            }
            str_list.back().append(pgn_string);
            if(i == cur_move_ind - 1){
                cur_move_end = str_list.back().size();
            }
        }
    }for(int i = 0; i < str_list.size(); i++){
        if(i == cur_move_row){
            for(int j = 0; j < cur_move_start; j++){
                mvwprintw(history_window, i + 1, j, "%c", str_list[i].at(j));
            }wattron(history_window, A_UNDERLINE);
            for(int j = cur_move_start; j < cur_move_end; j++){
                mvwprintw(history_window, i + 1, j, "%c", str_list[i].at(j));
            }wattroff(history_window, A_UNDERLINE);
            for(int j = cur_move_end; j < str_list[i].size(); j++){
                mvwprintw(history_window, i + 1, j, "%c", str_list[i].at(j));
            }continue;
        }
        mvwprintw(history_window, i + 1, 0, "%s", str_list[i].c_str());
    }box(history_window, 0, 0);
    wrefresh(history_window);
    refresh();
}
//ASSUMES THE PGN IS VALID
void import_pgn(char *pgn){
    char pgn_copy[MAX_PGN_LENGTH];
    strncpy(pgn_copy, pgn, MAX_PGN_LENGTH);
    parse_board(get_board_ptr(), STANDARD_STARTING_POS_FEN);
    reset_move_history();

    char *token = strtok(pgn_copy, " ");
    while(token){
        if(token[0] > '9'){
            uint16_t move = pgn_to_move(get_board_ptr(), token);
            if(!move)
                return;
            append_move(move);
            make_move(get_board_ptr(), move);
        }
        token = strtok(NULL, " ");
    }
}