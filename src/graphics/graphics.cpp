#include "graphics.h"

/*
    Initializes ncurses and all the windows
*/

void init_graphics(){
    initscr();
    clear();
    noecho();
    cbreak();
    init_colors();
    keypad(stdscr, 1);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    curs_set(0); //sets cursor invisible
    refresh();
    init_board_win();
    init_move_history();
    init_buttons();
    init_menu_options();
    initialize_timer();
    //draw_board();
    //draw_buttons();
}

/*
    The core analysis loop, handling user input for the board and window
    Loop for analysis mode
*/

void start_analysis_loop(){
    //no need for timer support
    nodelay(stdscr, FALSE);

    //Reset the board at the start of the game loop to the original position
    reset_move_history();
    reset_board_win();
    draw_all();
    draw_analysis_buttons();
    draw_all();
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

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void start_game_loop(){
    //need a timer
    nodelay(stdscr, TRUE);
    //Reset the board at the start of the game loop to the original position
    reset_move_history();
    reset_board_win();

    //draw_all();
    draw_local_buttons();
    draw_all();
    MEVENT event;
    int c;
    start_timer();
    while (1) {
        if(kbhit()){
            c = getch();
            if (c == KEY_MOUSE) {
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if(point_in_window(get_board_win(), event.x, event.y)){
                            board_receive_input(cursor_to_window_x(event.x), cursor_to_window_y(event.y));
                        }else{
                            button_receive_input_game(event.x, event.y);
                        }
                    }
                }
                draw_all();
            }
        }else{
            struct timespec remaining, request = {0, 5000L};
            nanosleep(&request, &remaining);
        }
        update_timer();
    }
    endwin();
}


void draw_all(){
    draw_board();
    draw_move_history();
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