#include "bitboard.h"

void clear_board(Board *board) {
    for (int i = 0; i < BB_SIZE; i++) {
        board->bitboards[i] = 0;
    }
    for (int i = 0; i < NUM_SQUARES; i++) {
        board->mailbox[i] = 0;
    }
}

void parse_board(Board *board, char *fen) {
    clear_board(board);
    while(*fen != EOF){
        
    }
}