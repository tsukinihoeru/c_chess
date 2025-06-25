#ifdef __cplusplus
extern "C" {
#endif

#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "../move_gen/bitboard.h"
#ifndef GRAPHICS_H
#define GRAPHICS_H

static const int SQUARE_HEIGHT = 6;
static const int SQUARE_WIDTH = 12;
static const int BOARD_HPAD = 2;
static const int BOARD_VPAD = 1;

extern char STARTING_POSITION_FEN[];
extern char STANDARD_STARTING_POS_FEN[];
extern const int MAX_PGN_LENGTH;

//start_menu.cpp
void start();
void start_menu_loop();

//timer.cpp
//later used to set up time control settings from menu selection
void draw_timer_windows();
void update_timer();
void initialize_timer();
void start_timer();

//graphics.c functions
void init_graphics();
void init_colors();
void start_analysis_loop();
void start_game_loop();

bool point_in_window(WINDOW *win, int px, int py);
int cursor_to_window_x(int cposx);
int cursor_to_window_y(int cposy);
void set_highlighted_square(int square);

void draw_all();

//board_window functions
void init_board_win();
void set_board_win(char *fen);
void reset_board_win();
void set_board_win_from_pgn(Board *other_board);
WINDOW* get_board_win();
Board* get_board_ptr();
void board_receive_input(int win_x, int win_y);
void draw_board();
bool square_highlighted(int square);
uint16_t get_engine_move();
void official_make_move(uint16_t move);
int board_get_side_to_move();

//move_history functions
void draw_move_history();
void init_move_history();
void reset_move_history();
void import_pgn(char *pgn);
bool can_undo_move();
bool can_redo_move();
uint16_t undo_move();
uint16_t redo_move();
void append_move(uint16_t move);

//button functions
void draw_analysis_buttons();
void draw_local_buttons();
void init_buttons();
void click_undo();
void click_redo();
void click_exit();
void button_receive_input(int cursor_x, int cursor_y);
void button_receive_input_game(int cursor_x, int cursor_y);

void draw_splash_screen();
void init_menu_options();

static const int splash_screen_hpad = 20;

#endif

#ifdef __cplusplus
} // closing brace for extern "C"
#endif