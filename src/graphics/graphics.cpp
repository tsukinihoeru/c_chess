#include "graphics.h"

int highlighted_square = -1;

void set_highlighted_square(int square){
    highlighted_square = square;
}

void init_graphics(){
    initscr();
    clear();
    noecho();
    cbreak();
    init_colors();
    keypad(stdscr, 1);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    refresh();
    init_board_win();
    init_move_history();
    init_buttons();
    draw_board();
    draw_buttons();
}

void start_game_loop(){
    MEVENT event;
    int c;
    while (1) {
        c = getch();
        if (c == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    if(point_in_window(get_board_win(), event.x, event.y)){
                        board_receive_input(cursor_to_window_x(event.x), cursor_to_window_y(event.y));
                    }else{
                        button_receive_input(event.x, event.y);
                    }
                }
            }
            draw_all();
        }
        else if (c == 10 || c == 'a') {
            click_exit();
        }else if(c == KEY_LEFT){
            click_undo();
            draw_all();
        }else if(c == KEY_RIGHT){
            click_redo();
            draw_all();
        }
    }
    endwin();
}

void draw_all(){
    draw_board();
    draw_move_history();
}

static const char splash_message[] = ">Press Any Key to Continue<";

void draw_splash_screen(WINDOW * win){
    for(int i = 0; i < splash_screen_art_length; i++){
        mvwprintw(win, i, splash_screen_hpad, "%s", splash_screen_art[i]);
    }
    struct timespec remaining, request = {0, 50000000L}; ;
    wmove(win, 20, splash_screen_hpad + 14);
    for(int i = 0; i < 27; i++){
        nanosleep(&request, &remaining);
        waddch(win, splash_message[i]);
        wrefresh(win);
    }
}

bool point_in_window(WINDOW *win, int px, int py){
    int beg_x, beg_y;
    int size_x, size_y;
    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, size_y, size_x);
    refresh();
    return px >= beg_x && px < beg_x + size_x && py >= beg_y && py < beg_y + size_y;
}

int cursor_to_window_x(int cposx){
    int resx = (cposx - BOARD_HPAD);
    return resx;
}

int cursor_to_window_y(int cposy){
    int resy = (cposy - BOARD_VPAD);
    return resy;
}