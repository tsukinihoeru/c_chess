#include "graphics.h"
#include <stdlib.h>

const int num_buttons = 6;
WINDOW *buttons[num_buttons];
void (*button_funcs[num_buttons]) (void);
const int UNDO_BUTTON = 0, REDO_BUTTON = 1, EXIT_BUTTON = 2, FEN_BUTTON = 3, PGN_BUTTON = 4, ENGINE_BUTTON = 5;
const char button_labels[num_buttons][30] = {"UNDO MOVE", "REDO MOVE", "QUIT", "ENTER FEN", "ENTER PGN", "ENGINE"};
const int x_padding[num_buttons] = {5, 5, 3, 3, 3, 3};

const int SMALL_BUTTON_WIDTH = 19, SMALL_BUTTON_HEIGHT = 3;
const int BIG_BUTTON_WIDTH = 15;
const int UNDO_X = 100, UNDO_Y = 41;
const int REDO_X = 131, REDO_Y = 41;
const int EXIT_WIDTH = 10, EXIT_HEIGHT = 3;
const int EXIT_X = 100, EXIT_Y = 44;
const int FEN_X = 110, FEN_Y = 44;
const int PGN_X = 125, PGN_Y = 44;
const int ENG_X = 100, ENG_Y = 47;
void click_fen();
void click_pgn();
void click_engine();

void exit_curses_get_input(char *str, char *message, int lim);

void init_buttons(){
    buttons[UNDO_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, SMALL_BUTTON_WIDTH, UNDO_Y, UNDO_X);
    button_funcs[UNDO_BUTTON] = click_undo;
    buttons[REDO_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, SMALL_BUTTON_WIDTH, REDO_Y, REDO_X);
    button_funcs[REDO_BUTTON] = click_redo;
    buttons[EXIT_BUTTON] = newwin(EXIT_HEIGHT, EXIT_WIDTH, EXIT_Y, EXIT_X);
    button_funcs[EXIT_BUTTON] = click_exit;
    buttons[FEN_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, BIG_BUTTON_WIDTH, FEN_Y, FEN_X);
    button_funcs[FEN_BUTTON] = click_fen;
    buttons[PGN_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, BIG_BUTTON_WIDTH, PGN_Y, PGN_X);
    button_funcs[PGN_BUTTON] = click_pgn;
    buttons[ENGINE_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, BIG_BUTTON_WIDTH, ENG_Y, ENG_X);
    button_funcs[ENGINE_BUTTON] = click_engine;
}

void draw_buttons(){
    for(int i = 0; i < num_buttons; i++){
        mvwprintw(buttons[i], 1, x_padding[i], button_labels[i]);
        box(buttons[i], 0, 0);
        wrefresh(buttons[i]);
    }
    refresh();
}

void button_receive_input(int cursor_x, int cursor_y){
    for(int i = 0; i < num_buttons; i++){
        if(point_in_window(buttons[i], cursor_x, cursor_y)){
            (*button_funcs[i]) ();
        }
    }
}

void click_undo(){
    if(can_undo_move()){
        unmake_move(get_board_ptr(), undo_move());
    }
}

void click_redo(){
    if(can_redo_move()){
        make_move(get_board_ptr(), redo_move());
    }
}

void click_exit(){
    endwin();
    exit(0);
}

void click_fen(){
    char fen_str[65];
    exit_curses_get_input(fen_str, "ENTER FEN: ", 65);
    reset_move_history();
    set_board_win(fen_str);
}

void click_pgn(){
    char pgn_str[MAX_PGN_LENGTH];
    exit_curses_get_input(pgn_str, "ENTER PGN: ", MAX_PGN_LENGTH);
    import_pgn(pgn_str);
}

const char square[64][3] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

void click_engine(){
    uint16_t move = get_engine_move();
    int source = (move >> 10) & 0x3f;
    int dest = (move >> 4) & 0x3f;
    int flag = move & 0x0f;
    mvprintw(51, 0, "%s%s%d", square[source], square[dest], flag);
}

void exit_curses_get_input(char *str, char *message, int lim){
    def_prog_mode();
	endwin();
    printf("%s", message);
    fgets(str, lim, stdin);
    reset_prog_mode();
}

