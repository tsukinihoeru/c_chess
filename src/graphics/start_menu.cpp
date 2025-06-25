#include "graphics.h"

void start_menu_loop();

/*
    This function is called to start the TUI
    It handles transitions from menu screen to analysis board
    TODO: change it so that it only draws the splash screen 
    and have a separate function handle the transitions
*/
void start(){
    init_graphics();
    draw_splash_screen();
    getch();
    erase();
    start_menu_loop();
}

static const char splash_message[] = ">Press Any Key to Continue<";

const char splash_screen_art[17][55] = {
    "      # ###      /                                    ",
    "    /  /###  / #/                                     ",
    "   /  /  ###/  ##                                     ",
    "  /  ##   ##   ##                                     ",
    " /  ###        ##                                     ",
    "##   ##        ##  /##      /##       /###     /###   ",
    "##   ##        ## / ###    / ###     / #### / / #### /",
    "##   ##        ##/   ###  /   ###   ##  ###/ ##  ###/ ",
    "##   ##        ##     ## ##    ### ####     ####      ",
    "##   ##        ##     ## ########    ###      ###     ",
    "##  ##         ##     ## #######       ###      ###   ",
    " ## #       /  ##     ## ##              ###      ### ",    
    "  ###      /   ##     ## ####    /  /###  ## /###  ## ",
    "   ######/     ##     ##  ######/  / #### / / #### /  ",
    "     ###       ###    ##   #####      ###/     ###/   ",
    "                     /                                ",
    "                    /                                 "
};

void draw_splash_screen(){
    for(int i = 0; i < 17; i++){
        mvprintw(i + 2, splash_screen_hpad, "%s", splash_screen_art[i]);
    }
    struct timespec remaining, request = {0, 50000000L};
    move(20, splash_screen_hpad + 14);
    for(int i = 0; i < 27; i++){
        nanosleep(&request, &remaining);
        addch(splash_message[i]);
        refresh();
    }
}

//TODO: implement all the different game states listed below

const int num_ops = 4;
WINDOW *menu_options[num_ops];
void (*menu_funcs[num_ops]) (void);
const int PVP = 0, VS_ENGINE = 1, ANALYSIS = 2, QUIT = 3;
const char option_labels[num_ops][30] = {"Play Local", "Play VS Engine", "Analysis Mode", "Quit"};
const int option_width = 20;
const int option_height = 3;
const int option_x = 14;
const int option_y = 24;
const int offset_y = 4;

void click_quit();
void click_local();
void click_analysis();

void init_menu_options(){
    menu_options[PVP] = newwin(option_height, option_width, option_y + offset_y, option_x);
    menu_options[VS_ENGINE] = newwin(option_height, option_width, option_y + offset_y * 2, option_x);
    menu_options[ANALYSIS] = newwin(option_height, option_width, option_y + offset_y * 3, option_x);
    menu_options[QUIT] =  newwin(option_height, option_width, option_y + offset_y * 4, option_x);
    menu_funcs[PVP] = click_local;
    menu_funcs[VS_ENGINE] = click_local;
    menu_funcs[ANALYSIS] = click_analysis;
    menu_funcs[QUIT] = click_quit;
}

void receive_input(int cursor_x, int cursor_y){
    for(int i = 0; i < num_ops; i++){
        if(point_in_window(menu_options[i], cursor_x, cursor_y)){
            (*menu_funcs[i]) ();
        }
    }
}

void start_menu_loop(){
    //no need for timer
    nodelay(stdscr, FALSE);
    for(int i = 0; i < 17; i++){
        mvprintw(i + 2, splash_screen_hpad, "%s", splash_screen_art[i]);
    }
    refresh();
    for(int i = 0; i < num_ops; i++){
        mvwprintw(menu_options[i], 1, 3, option_labels[i]);
        box(menu_options[i], 0, 0);
        wrefresh(menu_options[i]);
    }
    refresh();
    MEVENT event;
    int c;
    while (1) {
        c = getch();
        if (c == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    receive_input(event.x, event.y);
                }
            }
        }
    }
    getch();
    erase();
    start_analysis_loop();
}

void click_analysis(){
    erase();
    start_analysis_loop();
}

void click_local(){
    start_game_loop();
}
void click_quit(){
    endwin();
    exit(0);
}