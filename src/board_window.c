#include "graphics.h"
#include "move_gen/bitboard.h"

typedef enum {
    UNSELECTED,
    PIECE_SELECTED
} Board_State;

FILE *debug_log;

WINDOW *board_win;
Board board;
int selected_square;
bool target_squares[64]; //the squares that the selected piece can attack
Board_State state;

int get_square_color(int piece, int i, int j);
void clear_highlights();
void select_piece(int square);
void set_target_squares(int square);
void try_make_move(int from, int to);
bool check_move_invalid(Board * board, uint16_t move);

void init_board_win(WINDOW *win){
    debug_log = fopen("log.txt", "w");
    board_win = win;
    board_win = newwin(8 * SQUARE_HEIGHT, 8 * SQUARE_WIDTH, BOARD_VPAD, BOARD_HPAD);
    init_magics();
    char opening_fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    parse_board(&board, opening_fen);
    clear_highlights();
    state = UNSELECTED;
    fprintf(debug_log, "JUST SHOW IT GOD DAMN IT \n");
}

void receive_input(int win_x, int win_y){ //win_x and win_y are window relative coordinates
    fprintf(debug_log, "received click... \n");
    int square = (7 - (win_y)/SQUARE_HEIGHT) * 8 + win_x / SQUARE_WIDTH;
    if(square < 0 || square > 63)
        return;
    fprintf(debug_log, "received click on square: %d\n", square);
    if(state == UNSELECTED){
        select_piece(square);
    }else if(state == PIECE_SELECTED){
        try_make_move(selected_square, square);
        clear_highlights();
        state = UNSELECTED;
    }
}

void draw_board(){
    for (int i = 7; i >= 0; i--) {
        for(int k = 0; k < 6; k++){
            wmove(board_win, (7 - i) * 6 + k, 0);
            for (int j = 0; j < 8; j++) {
                int piece = board.mailbox[i * 8 + j];
                int color = get_square_color(piece, i, j);
                wattron(board_win, COLOR_PAIR(color));
                if(piece){
                    wprintw(board_win, "%s", piece_art[(piece >> 1) - 2][k]);
                }
                else{
                    wprintw(board_win, "            ");
                }
                wattroff(board_win, COLOR_PAIR(color));
            }
        }
    }
    wrefresh(board_win);
    refresh();
}

bool square_highlighted(int square){
    return square == selected_square || target_squares[square];
}

void clear_highlights(){
    for(int i = 0; i < 64; i++)
        target_squares[i] = false;
    selected_square = -1;
}

void select_piece(int square){
    if(board.mailbox[square] && (board.mailbox[square] & 1) == board.side_to_move){
        selected_square = square;
        set_target_squares(square);
        state = PIECE_SELECTED;
    }
}

void set_target_squares(int square){
    uint16_t move_list[200];
    int num_moves = generate_moves(&board, move_list);
    for(int i = 0; i < num_moves; i++){
        if(check_move_invalid(&board, move_list[i]))
            continue;
        int source = (move_list[i] >> 10) & 0x3f;
        int dest = (move_list[i] >> 4) & 0x3f;
        if(source == square)
            target_squares[dest] = true;
    }
}

void try_make_move(int from, int to){
    uint16_t move_list[200];
    int num_moves = generate_moves(&board, move_list);
    for(int i = 0; i < num_moves; i++){
        if(check_move_invalid(&board, move_list[i]))
            continue;
        int source = (move_list[i] >> 10) & 0x3f;
        int dest = (move_list[i] >> 4) & 0x3f;
        if(from == source && to == dest){
            make_move(&board, move_list[i]);
            return;
        }
    }
}

bool check_move_invalid(Board * board, uint16_t move){
    make_move(board, move);
    if(move_invalid(board, move)){
        unmake_move(board, move);
        return true;
    }unmake_move(board, move);
    return false;
}