/*
    Manages the timer of both players, communicates with board_window to see which side runs
    Is updated every game loop
*/
#include <chrono>
#include "graphics.h"
using namespace std::chrono;

high_resolution_clock::time_point cycle_start_time, cycle_end_time;
// Time left recorded in seconds
double white_time_left, black_time_left;
double increment;

WINDOW *white_timer_window, *black_timer_window;
int white_timer_win_x = 100;
int white_timer_win_y = 41;
int black_timer_win_x = 115;
int black_timer_win_y = 41;
int timer_width = 15;
int timer_height = 4;

void draw_timer_windows(){
    //erase and redraw white timer window
    werase(white_timer_window);
    int white_seconds = ((int) white_time_left) % 60;
    int white_minutes = ((int) white_time_left) / 60;
    mvwprintw(white_timer_window, 1, 2, "White\n  %d:%02d", (int) white_minutes, (int) white_seconds);
    box(white_timer_window, 0, 0);
    wrefresh(white_timer_window);

    //erase and redraw black timer window
    werase(black_timer_window);
    int black_seconds = ((int) black_time_left) % 60;
    int black_minutes = ((int) black_time_left) / 60;
    mvwprintw(black_timer_window, 1, 2, "Black\n  %d:%02d", (int) black_minutes, (int) black_seconds);
    box(black_timer_window, 0, 0);
    wrefresh(black_timer_window);
    refresh();
}

void update_timer(){
    cycle_end_time = high_resolution_clock::now();
    double dur = duration<double>(cycle_end_time - cycle_start_time).count();
    cycle_start_time = high_resolution_clock::now();
    if(board_get_side_to_move()){ //black
        black_time_left -= dur;
    }else{
        white_time_left -= dur;
    }
    draw_timer_windows();
}

//later used to set up time control settings from menu selection
void initialize_timer(){
    white_time_left = 65;
    black_time_left = 99;
    increment = 0;
    white_timer_window = newwin(timer_height, timer_width, white_timer_win_y, white_timer_win_x);
    black_timer_window = newwin(timer_height, timer_width, black_timer_win_y, black_timer_win_x);
}

void start_timer(){
    draw_timer_windows();
    cycle_start_time = high_resolution_clock::now();
}