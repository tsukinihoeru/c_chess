#include "bitboard.h"
#include "bitboard_data.h"

#if defined(__APPLE__)
    #include <libkern/OSByteOrder.h>
    #define reverse(x) OSSwapInt64(x)
#elif defined(__linux__)
    #include <byteswap.h>
    #define reverse(x) __bswap_64(x)
#elif defined(_WIN32)
    #include <stdlib.h>
    #define reverse(x) _byteswap_uint64(x)
#else
    #error "Byte swapping function not defined for this platform."
#endif

static inline int pop_lsb(uint64_t *bitboard);
static inline uint16_t * add_moves(int source, uint64_t dests, int flag, uint16_t * move_list);
static inline uint16_t * add_promo_moves(int source, int dest, uint16_t * move_list);
static inline uint16_t * add_promo_cap_moves(int source, int dest, uint16_t * move_list);

const uint64_t k1 = (0x5555555555555555);
const uint64_t k2 = (0x3333333333333333);
const uint64_t k4 = (0x0f0f0f0f0f0f0f0f);
static inline uint64_t mirror(uint64_t x){
   x = ((x >> 1) & k1) +  2*(x & k1);
   x = ((x >> 2) & k2) +  4*(x & k2);
   x = ((x >> 4) & k4) + 16*(x & k4);
   return x;
}
//see if program can be optimized by removing these functions, prolly will do nothing
static inline uint64_t shift_up_one(uint64_t b){return b << 8;}
static inline uint64_t shift_down_one(uint64_t b){return b >> 8;}
static inline uint64_t shift_left_up_one(uint64_t b){return b << 7;}
static inline uint64_t shift_right_up_one(uint64_t b){return b << 9;}
static inline uint64_t shift_left_down_one(uint64_t b){return b >> 7;}
static inline uint64_t shift_right_down_one(uint64_t b){return b >> 9;}
static inline uint64_t hyp_quint(int source, uint64_t mask, uint64_t occupied_board){
    return (((mask & occupied_board) - occupy_square[source] * 2) ^
            reverse(reverse(mask & occupied_board) - reverse(occupy_square[source]) * 2)) & mask;
}
static inline uint64_t hyp_quint_horiz(int source, uint64_t mask, uint64_t occupied_board){
    return (((mask & occupied_board) - occupy_square[source] * 2) ^
            mirror(mirror(mask & occupied_board) - mirror(occupy_square[source]) * 2)) & mask;
}
const int undo_left_capture[2] = {-7, 7};
const int undo_right_capture[2] = {-9, 9};
const int undo_single_push[2] = {-8, 8};
const int undo_double_push[2] = {-16, 16};
const uint64_t double_push_mask[2] = {0x00000000ff000000,  0x000000ff00000000};
const uint64_t promotion_rank[2] = {0xff00000000000000, 0x00000000000000ff};
const uint64_t non_promotion_mask[2] = {0x00ffffffffffffff, 0xffffffffffffff00};
uint64_t *rook_magic_ptrs[64];
uint64_t *bishop_magic_ptrs[64];
uint64_t *attack_table;
const int check_ks_castling[2] = {8, 2};
const uint64_t ks_castling_occupy[2] = {0x60, 0x6000000000000000};
const uint64_t qs_castling_occupy[2] = {0xe, 0xe00000000000000};
const int check_qs_castling[2] = {4, 1};
const uint16_t ks_castle_move [2] = {4194, 62434};
const uint16_t qs_castle_move [2] = {4131, 62371};

int generate_moves(Board *board, uint16_t * move_list){
    uint16_t * og_list = move_list;
    uint64_t b1, b2, b3;
    int source;
    uint64_t us = board->bitboards[board->side_to_move];
    uint64_t them = board->bitboards[!board->side_to_move];
    uint64_t occupied = us | them;
    uint64_t empty = ~occupied;
    //pawn moves, if branch mispredictions happen often, program can be sped up by branchless programming
    //have an array store both results of shiftupone and shiftdownone, then use board->side to move as index
    b1 = board->bitboards[PAWN_BOARD] & us;
    //en passant first
    b2 = b1 & en_passant_lookup[board->history[board->ply].en_passant_square];
    while(b2){
        int source = pop_lsb(&b2);
        *move_list++ = (source << 10) | ((board->history[board->ply].en_passant_square - undo_single_push[board->side_to_move]) << 4) | EN_PASSANT_FLAG;
    }
    b2 = board->side_to_move ? shift_left_down_one(b1) : shift_left_up_one(b1);
    b2 &= them;
    b3 = b2 & non_promotion_mask[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
        *move_list++ = ((dest + undo_left_capture[board->side_to_move]) << 10) | (dest << 4) | CAPTURE_FLAG;
    }
    b3 = b2 & promotion_rank[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
        move_list = add_promo_cap_moves(dest + undo_left_capture[board->side_to_move], dest, move_list);
    }
    b2 = board->side_to_move ? shift_right_down_one(b1) : shift_right_up_one(b1);
    b2 &= them;
    b3 = b2 & non_promotion_mask[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
        *move_list++ = ((dest + undo_right_capture[board->side_to_move]) << 10) | (dest << 4) | CAPTURE_FLAG;
    }
    b3 = b2 & promotion_rank[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
        move_list = add_promo_cap_moves(dest + undo_right_capture[board->side_to_move], dest, move_list);
    }
    b2 = board->side_to_move ? shift_down_one(b1) : shift_up_one(b1);
    b2 &= empty;
    //double pawn push
    b3 = board->side_to_move ? shift_down_one(b2) : shift_up_one(b2);
    b3 &= empty & double_push_mask[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
         *move_list++ = ((dest + undo_double_push[board->side_to_move]) << 10) | (dest << 4) | DOUBLE_PUSH_FLAG;
    }
    //non promotions
    b3 = b2 & non_promotion_mask[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
        *move_list++ = ((dest + undo_single_push[board->side_to_move]) << 10) | (dest << 4) | QUIET_FLAG;
    }
    b3 = b2 & promotion_rank[board->side_to_move];
    while(b3){
        int dest = pop_lsb(&b3);
        move_list = add_promo_moves(dest + undo_single_push[board->side_to_move], dest, move_list);
    }

    //knight moves w lookup table
    b1 = board->bitboards[KNIGHT_BOARD] & us;
    while(b1){
        source = pop_lsb(&b1);
        b2 = knight_move_lookup[source];
        move_list = add_moves(source, b2 & empty, QUIET_FLAG, move_list);
        move_list = add_moves(source, b2 & them, CAPTURE_FLAG, move_list);
    }

    //normal king moves w lookup, no loop cause u cant have multiple kings
    b1 = board->bitboards[KING_BOARD] & us;
    source = pop_lsb(&b1);
    b2 = king_move_lookup[source];
    move_list = add_moves(source, b2 & empty, QUIET_FLAG, move_list);
    move_list = add_moves(source, b2 & them, CAPTURE_FLAG, move_list);
    //castling, wo regard to checks
    //idea for not branching at this stage -> use array, where 0 is null move 1 is castle
    if((check_ks_castling[board->side_to_move] & board->history[board->ply].castling_rights) && !(occupied & ks_castling_occupy[board->side_to_move])){
        *move_list++ = ks_castle_move[board->side_to_move];
    }
    if((check_qs_castling[board->side_to_move] & board->history[board->ply].castling_rights) && !(occupied & qs_castling_occupy[board->side_to_move])){
        *move_list++ = qs_castle_move[board->side_to_move];
    }
    //bishop moves
    b1 = (board->bitboards[BISHOP_BOARD] | board->bitboards[QUEEN_BOARD]) & us;
    while(b1){
        source = pop_lsb(&b1);
        b3 = ((occupied & bishop_occupancy_mask[source]) * bishop_magic[source]) >> bishop_magic_shift[source]; //magic_index
        b2 = bishop_magic_ptrs[source][b3];
        move_list = add_moves(source, b2 & empty, QUIET_FLAG, move_list);
        move_list = add_moves(source, b2 & them, CAPTURE_FLAG, move_list);
    }
    //rook moves
    b1 = (board->bitboards[ROOK_BOARD] | board->bitboards[QUEEN_BOARD]) & us;
    while(b1){
        source = pop_lsb(&b1);
        b3 = ((occupied & rook_occupancy_mask[source]) * rook_magic[source]) >> rook_magic_shift[source]; //magic_index
        b2 = rook_magic_ptrs[source][b3];
        move_list = add_moves(source, b2 & empty, QUIET_FLAG, move_list);
        move_list = add_moves(source, b2 & them, CAPTURE_FLAG, move_list);
    }

    return (int) (move_list - og_list);
};

static inline uint16_t * add_moves(int source, uint64_t dests, int flag, uint16_t * move_list){
    while(dests){
        int dest = pop_lsb(&dests);
        *move_list++ = (source << 10) | (dest << 4) | flag;
    }
    return move_list;
}

static inline uint16_t * add_promo_moves(int source, int dest, uint16_t * move_list){
    *move_list++ = (source << 10) | (dest << 4) | QUEEN_PROMO_FLAG;
    *move_list++ = (source << 10) | (dest << 4) | KNIGHT_PROMO_FLAG;
    *move_list++ = (source << 10) | (dest << 4) | BISHOP_PROMO_FLAG;
    *move_list++ = (source << 10) | (dest << 4) | ROOK_PROMO_FLAG;
    return move_list;
}

static inline uint16_t * add_promo_cap_moves(int source, int dest, uint16_t * move_list){
    *move_list++ = (source << 10) | (dest << 4) | QUEEN_PROMO_CAP_FLAG;
    *move_list++ = (source << 10) | (dest << 4) | KNIGHT_PROMO_CAP_FLAG;
    *move_list++ = (source << 10) | (dest << 4) | BISHOP_PROMO_CAP_FLAG;
    *move_list++ = (source << 10) | (dest << 4) | ROOK_PROMO_CAP_FLAG;
    return move_list;
}

static inline int pop_lsb(uint64_t *bitboard) {
    int lsb_index = __builtin_ctzll(*bitboard);
    (*bitboard) = (*bitboard) & (*bitboard - 1);
    return lsb_index; 
}

int blocker_squares [15];
int get_occupancy_boards(uint64_t**occupancy_boards, uint64_t mask){
    int size = 0;
    for(int i = 0; i < 64; i++){
        if((mask >> i) & 1){
            blocker_squares[size] = i;
            size++;
        }
    }
    int power_size = 1 << size;
    *occupancy_boards = malloc(power_size * sizeof(uint64_t));          
    for(int i = 0; i < power_size; i++){
        (*occupancy_boards)[i] = 0;
        for(int j = 0; j < size; j++){
            uint64_t bit = (i >> j) & 1;
            (*occupancy_boards)[i] |= bit << blocker_squares[j];
        }
    }
    return power_size;
}

void init_magics(){
    attack_table = malloc(121504 * sizeof(uint64_t));
    int attack_index = 0;
    uint64_t *occupancy_boards;
    for(int i = 0; i < 64; i++){
        rook_magic_ptrs[i] = &attack_table[attack_index];
        int power_size = get_occupancy_boards(&occupancy_boards, rook_occupancy_mask[i]);
        for(int j = 0; j < power_size; j++){
            uint64_t occupied_board = occupancy_boards[j];
            uint64_t attack_board = hyp_quint(i, file_masks[source_to_file[i]], occupied_board)
                                    | hyp_quint_horiz(i, rank_masks[source_to_rank[i]], occupied_board);
            occupied_board *= rook_magic[i];
            occupied_board >>= rook_magic_shift[i];
            rook_magic_ptrs[i][occupied_board] = attack_board;
            attack_index++;
        }
        free(occupancy_boards);
    }
    for(int i = 0; i < 64; i++){
        bishop_magic_ptrs[i] = &attack_table[attack_index];
        int power_size = get_occupancy_boards(&occupancy_boards, bishop_occupancy_mask[i]);
        for(int j = 0; j < power_size; j++){
            uint64_t occupied_board = occupancy_boards[j];
            uint64_t attack_board = hyp_quint(i, diagonal_masks[source_to_diagonal[i]], occupied_board)
                                    | hyp_quint(i, antidiagonal_masks[source_to_antidiagonal[i]], occupied_board);
            occupied_board *= bishop_magic[i];
            occupied_board >>= bishop_magic_shift[i];
            bishop_magic_ptrs[i][occupied_board] = attack_board;
            attack_index++;
        }
        free(occupancy_boards);
    }
}