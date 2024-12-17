#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#ifndef BITBOARD_H
#define BITBOARD_H

#define WHITE 0
#define BLACK 1
#define PAWN_BOARD 2
#define KNIGHT_BOARD 3
#define BISHOP_BOARD 4
#define ROOK_BOARD 5
#define QUEEN_BOARD 6
#define KING_BOARD 7

static const int BB_SIZE = 8;
static const int NUM_SQUARES = 64;

typedef struct Board {
    int mailbox[64];
    uint64_t bitboards[8];
} Board;

/// @brief sets all values in the board to 0 (empty)
/// @param board the board to clear
void clear_board(Board *board);

/// @brief sets board based on fen string
void parse_board(Board *board, char *fen);

/// @brief sets square of board to contain piece
void add_piece(Board *board, int piece, int square);
void print_board(Board *board);

static const uint64_t occupy_square[64] = {
    0x1, 0x2, 0x4, 0x8,
    0x10, 0x20, 0x40, 0x80,
    0x100, 0x200, 0x400, 0x800,
    0x1000, 0x2000, 0x4000, 0x8000,
    0x10000, 0x20000, 0x40000, 0x80000,
    0x100000, 0x200000, 0x400000, 0x800000,
    0x1000000, 0x2000000, 0x4000000, 0x8000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x100000000, 0x200000000, 0x400000000, 0x800000000,
    0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000,
    0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000,
    0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000,
    0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000,
    0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000,
    0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000,
    0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000};

#endif