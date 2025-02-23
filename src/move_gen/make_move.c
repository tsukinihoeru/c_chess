#include "bitboard.h"

const int castling_shenanigans[64] = {
    -5, -1, -1, -1, -13, -1, -1, -9,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -2, -1, -1, -1, -4, -1, -1, -3,
};
const int check_capture = 0b0100;
const int check_promo = 0b1000;
const int promo_mask = 0b0011;

static inline void move_piece(Board *board, int source, int dest);
static inline void remove_piece(Board *board, int square);

void make_move(Board *board, uint16_t move){
    board->zobrist_hash ^= castling_hash[board->history[board->ply].castling_rights];
    board->zobrist_hash ^= en_passant_hash[board->history[board->ply].en_passant_square];

    int source = (move >> 10) & 0x3f;
    int dest = (move >> 4) & 0x3f;
    int flag = move & 0x0f;

    int en_passant_square = (flag == DOUBLE_PUSH_FLAG) ? dest : 0;
    int captured_piece = 0;
    uint8_t new_castling_rights = board->history[board->ply].castling_rights & castling_shenanigans[source] & castling_shenanigans[dest];
   
    if(flag & check_capture){
        int capture_location = (flag == EN_PASSANT_FLAG) ? board->history[board->ply].en_passant_square : dest;
        captured_piece = board->mailbox[capture_location];
        remove_piece(board, capture_location);
    }
    move_piece(board, source, dest);
    if(flag & check_promo){
        board->bitboards[PAWN_BOARD] &= ~occupy_square[dest];
        board->zobrist_hash ^= piece_hash[(PAWN_BOARD << 1) + board->side_to_move][dest];
        int piece = ((flag & promo_mask) + 3);
        board->bitboards[piece] |= occupy_square[dest];
        board->mailbox[dest] = (piece << 1) + board->side_to_move;
        board->zobrist_hash ^= piece_hash[board->mailbox[dest]][dest];
    }
    else if(flag == KS_CASTLE_FLAG)
        move_piece(board, source + 3, source + 1);
    else if(flag == QS_CASTLE_FLAG)
        move_piece(board, source - 4, source - 1);
    board->side_to_move = !board->side_to_move;
    board->ply++;
    board->history[board->ply].castling_rights = new_castling_rights;
    board->history[board->ply].captured_piece = captured_piece;
    board->history[board->ply].en_passant_square = en_passant_square;
    board->zobrist_hash ^= castling_hash[new_castling_rights];
    board->zobrist_hash ^= en_passant_hash[en_passant_square];
    board->zobrist_hash ^= color_hash;
}

void unmake_move(Board *board, uint16_t move){
    board->zobrist_hash ^= castling_hash[board->history[board->ply].castling_rights];
    board->zobrist_hash ^= en_passant_hash[board->history[board->ply].en_passant_square];
    int source = (move >> 10) & 0x3f;
    int dest = (move >> 4) & 0x3f;
    int flag = move & 0x0f;
    move_piece(board, dest, source);
    if(flag & check_capture){
        int piece = board->history[board->ply].captured_piece;
        int capture_location = (flag == EN_PASSANT_FLAG) ? board->history[board->ply - 1].en_passant_square : dest;
        board->bitboards[piece >> 1] |= occupy_square[capture_location];
        board->bitboards[piece & 1] |= occupy_square[capture_location];
        board->mailbox[capture_location] = piece;
        board->zobrist_hash ^= piece_hash[piece][capture_location];
    }
    else if(flag == KS_CASTLE_FLAG)
        move_piece(board, source + 1, source + 3);
    else if(flag == QS_CASTLE_FLAG)
        move_piece(board, source - 1, source - 4);
    if(flag & check_promo){
        board->zobrist_hash ^= piece_hash[board->mailbox[source]][source];
        board->bitboards[board->mailbox[source] >> 1] &= ~occupy_square[source];
        board->bitboards[PAWN_BOARD] |= occupy_square[source];
        board->mailbox[source] = (PAWN_BOARD << 1) + !board->side_to_move;
        board->zobrist_hash ^= piece_hash[(PAWN_BOARD << 1) + board->side_to_move][source];
    }
    board->side_to_move = !board->side_to_move;
    board->ply--;
    board->zobrist_hash ^= castling_hash[board->history[board->ply].castling_rights];
    board->zobrist_hash ^= en_passant_hash[board->history[board->ply].en_passant_square];
    board->zobrist_hash ^= color_hash;
}

int castle_valid_check[6][3] = {{0, 0, 0},{0, 0, 0},{60, 61, 62}, {60, 59, 58},{4,5,6},{2,3,4}};
int move_invalid(Board *board, uint16_t move){
    int flag = move & 0x0f;
    if((flag) != KS_CASTLE_FLAG && (flag) != QS_CASTLE_FLAG){
        return square_attacked(board,  __builtin_ctzll(board->bitboards[KING_BOARD] & board->bitboards[!board->side_to_move]));
    }int check = flag + board->side_to_move * 2;
    return (square_attacked(board, castle_valid_check[check][0]) || 
             square_attacked(board, castle_valid_check[check][1]) || 
             square_attacked(board, castle_valid_check[check][2]));
}

static inline void move_piece(Board *board, int source, int dest){
    board->zobrist_hash ^= piece_hash[board->mailbox[source]][source] ^ piece_hash[board->mailbox[source]][dest];
    uint64_t mask = occupy_square[source] | occupy_square[dest];
    board->bitboards[board->mailbox[source] & 1] ^= mask;
    board->bitboards[board->mailbox[source] >> 1] ^= mask;
    board->mailbox[dest] = board->mailbox[source];
    board->mailbox[source] = 0;
}

static inline void remove_piece(Board *board, int square){
    board->zobrist_hash ^= piece_hash[board->mailbox[square]][square];
    board->bitboards[board->mailbox[square] & 1] &= ~occupy_square[square];
    board->bitboards[board->mailbox[square] >> 1] &= ~occupy_square[square];
    board->mailbox[square] = 0;
}