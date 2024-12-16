#include <stdint.h>
#include <stdio.h>

#ifndef BITBOARD_H
#define BITBOARD_H

const int BB_SIZE = 8;
const int NUM_SQUARES = 64;

struct Board {
    int mailbox[64];
    uint64_t bitboards[8];
};

void clear_board(Board *board);
void parse_board(Board *board, char *fen);

#endif