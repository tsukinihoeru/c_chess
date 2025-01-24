#include "graphics.h"

const int num_buttons = 2;
WINDOW *buttons[num_buttons];
void (*button_funcs[num_buttons]) (void);
const int UNDO_BUTTON = 0, REDO_BUTTON = 1;

const int SMALL_BUTTON_WIDTH = 19, SMALL_BUTTON_HEIGHT = 3;
const int UNDO_X = 100, UNDO_Y = 42;
const int REDO_X = 131, REDO_Y = 42;

void init_buttons(){
    buttons[UNDO_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, SMALL_BUTTON_WIDTH, UNDO_Y, UNDO_X);
    button_funcs[UNDO_BUTTON] = click_undo;
    buttons[REDO_BUTTON] = newwin(SMALL_BUTTON_HEIGHT, SMALL_BUTTON_WIDTH, REDO_Y, REDO_X);
    button_funcs[REDO_BUTTON] = click_redo;
}

void draw_buttons(){
    mvwprintw(buttons[UNDO_BUTTON], 1, 5, "UNDO MOVE");
    box(buttons[UNDO_BUTTON], 0, 0);
    wrefresh(buttons[UNDO_BUTTON]);
    mvwprintw(buttons[REDO_BUTTON], 1, 5, "REDO MOVE");
    box(buttons[REDO_BUTTON], 0, 0);
    wrefresh(buttons[REDO_BUTTON]);
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

void exit_curses_get_fen_input(char *str, int lim){
    def_prog_mode();
	endwin();
    printf("Enter FEN: ");
    fgets(str, lim, stdin);
    reset_prog_mode();
}