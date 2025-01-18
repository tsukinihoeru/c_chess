#include "graphics.h"

void exit_curses_get_fen_input(char *str, int lim){
    def_prog_mode();
	endwin();
    printf("Enter FEN: ");
    fgets(str, lim, stdin);
    reset_prog_mode();
}