#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BITBOARD_H
#define BITBOARD_H

extern const int BB_SIZE;
extern const int NUM_SQUARES;
extern const int WHITE;
extern const int BLACK;
extern const int PAWN_BOARD;
extern const int KNIGHT_BOARD;
extern const int BISHOP_BOARD;
extern const int ROOK_BOARD;
extern const int QUEEN_BOARD;
extern const int KING_BOARD;

extern const int QUIET_FLAG;
extern const int DOUBLE_PUSH_FLAG;
extern const int CAPTURE_FLAG;

extern const int KNIGHT_PROMO_FLAG;
extern const int BISHOP_PROMO_FLAG;
extern const int ROOK_PROMO_FLAG;
extern const int QUEEN_PROMO_FLAG;
extern const int KNIGHT_PROMO_CAP_FLAG;
extern const int BISHOP_PROMO_CAP_FLAG;
extern const int ROOK_PROMO_CAP_FLAG;
extern const int QUEEN_PROMO_CAP_FLAG;

typedef struct Board {
    int side_to_move;
    int mailbox[64];
    uint64_t bitboards[8];
} Board;

void init_magics();
int generate_moves(Board *board, uint16_t * move_list);

/// @brief sets board based on fen string
void parse_board(Board *board, char *fen);
void clear_board(Board *board);
void add_piece(Board *board, int piece, int square);

void print_board(Board *board);
void print_move(uint16_t move);
void print_u64(uint64_t board);

#endif