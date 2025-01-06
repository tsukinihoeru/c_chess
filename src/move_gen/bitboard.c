#include "bitboard.h"
#include "bitboard_data.h"


static inline int pop_lsb(uint64_t *bitboard);
static inline uint16_t * add_moves(int source, uint64_t dests, int flag, uint16_t * move_list);

void generate_moves(Board *board, uint16_t * move_list){
    uint64_t b1, b2, b3;
    int source;
    uint64_t empty = ~(board->bitboards[WHITE] & board->bitboards[BLACK]);

    b1 = board->bitboards[KNIGHT_BOARD] & board->bitboards[board->side_to_move];
    while(b1){
        source = pop_lsb(&b1);
        b2 = knight_move_lookup[source];
        move_list = add_moves(source, b2 & empty, QUIET_FLAG, move_list);
        move_list = add_moves(source, b2 & board->bitboards[!board->side_to_move], CAPTURE_FLAG, move_list);
    }
    //return move_list;
};

static inline uint16_t * add_moves(int source, uint64_t dests, int flag, uint16_t * move_list){
    while(dests){
        int dest = pop_lsb(&dests);
        *move_list++ = (source << 10) | (dest << 4) | flag;
    }
    return move_list;
}

void add_piece(Board *board, int piece, int square){
    board->bitboards[piece & 1] |= occupy_square[square];
    board->bitboards[piece >> 1] |= occupy_square[square];
    board->mailbox[square] = piece;
}


static inline int pop_lsb(uint64_t *bitboard) {
    int lsb_index = __builtin_ctzll(*bitboard);
    (*bitboard) = (*bitboard) & (*bitboard - 1);
    return lsb_index; 
}

