#include "graphics.h"
#include "../engine/engine.h"

const char piece_art[6][SQUARE_HEIGHT][SQUARE_WIDTH + 1] = {
    {
        "            ",
        "     _      ",
        "    (_)     ",
        "   (___)    ",
        "   _|_|_    ",
        "  (_____)   "
    },{
        "    |\\_     ",
        "   /  .\\_   ",
        "  |   ___)  ",
        "   \\ = |    ",
        "   /____\\   ",
        "  [______]  "
    },{
        "     _0     ",
        "   / //\\    ",
        "  {     }   ",
        "   \\   /    ",
        "   _|_|_    ",
        "  (_____)   "
    },{
        "  __  _  _  ",
        " |________| ",
        " \\__ ____ / ",
        "  |____|_|  ",
        "  |__|___|  ",
        " [________] "
    },{
        "    _()_    ",
        "  _/____\\_  ",
        "  \\______/  ",
        "   (____)   ",
        "    |__|    ",
        "  (______)  "
    },{
        "    _++_    ",
        "  _/____\\_  ",
        "  \\______/  ",
        "   (____)   ",
        "    |__|    ",
        "  (______)  "
    }
};

typedef enum {
    UNSELECTED,
    PIECE_SELECTED
} Board_State;

WINDOW *board_win;
Board board;
Engine window_engine;
int selected_square;
bool target_squares[64]; //the squares that the selected piece can attack
Board_State state;

char STARTING_POSITION_FEN[65] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
char STANDARD_STARTING_POS_FEN[65] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
int get_square_color(int piece, int i, int j);
void clear_highlights();
void select_piece(int square);
void set_target_squares(int square);
void try_make_move(int from, int to);
bool check_move_invalid(Board * board, uint16_t move);

void init_board_win(){
    board_win = newwin(8 * SQUARE_HEIGHT, 8 * SQUARE_WIDTH, BOARD_VPAD, BOARD_HPAD);
    init_magics();
    init_zobrist_keys();
    parse_board(&board, STARTING_POSITION_FEN);
    window_engine.board = &board;
    clear_highlights();
    state = UNSELECTED;
}

void set_board_win(char *fen){
    parse_board(&board, fen);
    clear_highlights();
    state = UNSELECTED;
    int i = 0;
    for(; fen[i] != '\0'; i++){
        STARTING_POSITION_FEN[i] = fen[i];
    }STARTING_POSITION_FEN[i] = '\0';
}

void reset_board_win(){
    set_board_win(STANDARD_STARTING_POS_FEN);
}

void set_board_win_from_pgn(Board *other_board){
    set_board_win(STANDARD_STARTING_POS_FEN);
    copy_board(other_board, &board);
}

WINDOW* get_board_win(){
    return board_win;
}

Board* get_board_ptr(){
    return &board;
}

void board_receive_input(int win_x, int win_y){ //win_x and win_y are window relative coordinates
    int square = (7 - (win_y)/SQUARE_HEIGHT) * 8 + win_x / SQUARE_WIDTH;
    if(square < 0 || square > 63)
        return;
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
           official_make_move(move_list[i]);
            return;
        }
    }
}

void official_make_move(uint16_t move){
    make_move(&board, move);
    append_move(move);
}

bool check_move_invalid(Board * board, uint16_t move){
    make_move(board, move);
    if(move_invalid(board, move)){
        unmake_move(board, move);
        return true;
    }unmake_move(board, move);
    return false;
}

uint16_t get_engine_move(){
    return window_engine.iterative_deepening(7);
}