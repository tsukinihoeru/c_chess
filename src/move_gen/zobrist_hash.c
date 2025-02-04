#include "bitboard.h"

uint64_t s = 1070372;

uint64_t rand64(){
    s ^= s >> 12; s ^= s << 25; s ^= s >> 27;
    return s * 2685821657736338717LL;
}

uint64_t piece_hash[16][64];
uint64_t castling_hash[16];
uint64_t color_hash;
uint64_t en_passant_hash[40];

void init_zobrist_keys(){
    for(int piece = 4; piece < 16; piece++){
        for(int square = 0; square < 64; square++){
            piece_hash[piece][square] = rand64();
        }
    }for(int i = 0; i < 16; i++)
        castling_hash[i] = rand64();
    color_hash = rand64();
    for(int i = 0; i < 40; i++)
        en_passant_hash[i] = rand64();
}