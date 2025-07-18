#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BITBOARD_H
#define BITBOARD_H

extern const int BB_SIZE, NUM_SQUARES;
extern const int WHITE, BLACK, PAWN_BOARD, KNIGHT_BOARD, BISHOP_BOARD, ROOK_BOARD, QUEEN_BOARD, KING_BOARD;

extern const int QUIET_FLAG, DOUBLE_PUSH_FLAG, KS_CASTLE_FLAG, QS_CASTLE_FLAG, CAPTURE_FLAG, EN_PASSANT_FLAG,
                 KNIGHT_PROMO_FLAG, BISHOP_PROMO_FLAG, ROOK_PROMO_FLAG, QUEEN_PROMO_FLAG, KNIGHT_PROMO_CAP_FLAG,
                 BISHOP_PROMO_CAP_FLAG, ROOK_PROMO_CAP_FLAG, QUEEN_PROMO_CAP_FLAG;

extern const int WKS_CASTLING_RIGHTS, WQS_CASTLING_RIGHTS, BKS_CASTLING_RIGHTS, BQS_CASTLING_RIGHTS;
extern const uint64_t occupy_square[64];

extern uint64_t piece_hash[16][64];
extern uint64_t castling_hash[16];
extern uint64_t color_hash;
extern uint64_t en_passant_hash[40];

typedef struct game_state{
    uint8_t castling_rights;
    int en_passant_square;
    int captured_piece;
} game_state;

typedef struct Board {
    int side_to_move;
    int mailbox[64];
    uint64_t bitboards[8];
    game_state history[400];
    int ply;
    uint64_t zobrist_hash;
} Board;

void init_magics();
void init_zobrist_keys();
int generate_moves(Board *board, uint16_t * move_list);
int generate_captures(Board *board, uint16_t * move_list);
int square_attacked(Board *board, int square);
int is_checkmate(Board *board);

void make_move(Board *board, uint16_t move);
void unmake_move(Board *board, uint16_t move);
int move_invalid(Board *board, uint16_t move);
int position_in_check(Board *board);

void parse_board(Board *board, char *fen);
void copy_board(Board *src, Board *dest);
void clear_board(Board *board);
void add_piece(Board *board, int piece, int square);

void print_board(Board *board);
void print_move(uint16_t move);
void print_u64(uint64_t board);
uint64_t perft(Board *board, int depth);

void get_move_pgn(Board *board, uint16_t move, char * pgn);
uint16_t pgn_to_move(Board *board, char *pgn_token);

#endif

#ifdef __cplusplus
} // closing brace for extern "C"
#endif