#include "bitboard.h"

const int BB_SIZE = 8;
const int NUM_SQUARES = 64;
const int WHITE = 0, BLACK = 1, PAWN_BOARD = 2, KNIGHT_BOARD = 3, BISHOP_BOARD = 4, 
          ROOK_BOARD = 5, QUEEN_BOARD = 6, KING_BOARD = 7;

const int QUIET_FLAG = 0, DOUBLE_PUSH_FLAG = 1, KS_CASTLE_FLAG = 2, QS_CASTLE_FLAG = 3, CAPTURE_FLAG = 4, 
          EN_PASSANT_FLAG = 5, KNIGHT_PROMO_FLAG = 8, BISHOP_PROMO_FLAG = 9, ROOK_PROMO_FLAG = 10, 
          QUEEN_PROMO_FLAG = 11, KNIGHT_PROMO_CAP_FLAG = 12, BISHOP_PROMO_CAP_FLAG = 13, 
          ROOK_PROMO_CAP_FLAG = 14, QUEEN_PROMO_CAP_FLAG = 15;

const int WKS_CASTLING_RIGHTS = 8, WQS_CASTLING_RIGHTS = 4, BKS_CASTLING_RIGHTS = 2, BQS_CASTLING_RIGHTS = 1;

const uint64_t occupy_square[64] = {
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
    0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000
};

void clear_board(Board *board) {
    for (int i = 0; i < BB_SIZE; i++) 
        board->bitboards[i] = 0;
    for (int i = 0; i < NUM_SQUARES; i++) 
        board->mailbox[i] = 0;
    board->ply = 0;
    board->history[0].castling_rights = 0;
    board->history[0].en_passant_square = 0;
    board->history[0].captured_piece = 0;
    board->zobrist_hash = 0;
}

//small helper function for parse_board
int fen_char_to_piece(char c){
    int color = BLACK;
    if(isupper(c))
        color = WHITE;
    c = toupper(c);
    int piece_val[100];
    piece_val['P'] = PAWN_BOARD, piece_val['N'] = KNIGHT_BOARD, piece_val['B'] = BISHOP_BOARD, 
    piece_val['R'] = ROOK_BOARD, piece_val['Q'] = QUEEN_BOARD, piece_val['K'] = KING_BOARD;
    return piece_val[(int) c] * 2 + color;   
}

//Errors with invalid fen, use separate function to validate fen
void parse_board(Board *board, char *fen) {
    clear_board(board);
    int rank = 7;
    int file = 0;
    while(*fen != '\0'){
        if(*fen == '/'){
            rank--;
            file = 0;
        }else if(*fen > '0' && *fen < '9') { 
            file += *fen - '0';
        }else if(rank >= 0 && file < 8){
            int piece = fen_char_to_piece(*fen);
            add_piece(board, piece, rank * 8 + file);
            file++;
        }else if(*fen == 'w'){
            board->side_to_move = WHITE;
        }else if(*fen == 'b'){
            board->side_to_move = BLACK;
            board->zobrist_hash ^= color_hash;
        }else if(*fen == 'K'){
            board->history[board->ply].castling_rights |= WKS_CASTLING_RIGHTS;
        }else if(*fen == 'Q'){
            board->history[board->ply].castling_rights |= WQS_CASTLING_RIGHTS;
        }else if(*fen == 'k'){
            board->history[board->ply].castling_rights |= BKS_CASTLING_RIGHTS;
        }else if(*fen == 'q')
            board->history[board->ply].castling_rights |= BQS_CASTLING_RIGHTS;
        fen++;
    }
}

void add_piece(Board *board, int piece, int square){
    board->bitboards[piece & 1] |= occupy_square[square];
    board->bitboards[piece >> 1] |= occupy_square[square];
    board->mailbox[square] = piece;
    board->zobrist_hash ^= piece_hash[piece][square];
}

const char square[64][3] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

void print_move(uint16_t move){
    int source = (move >> 10) & 0x3f;
    int dest = (move >> 4) & 0x3f;
    int flag = move & 0x0f;
    printf("%s%s%d", square[source], square[dest], flag);
}

char piece_arr[16] = {'.', '1', '2', '3', 'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'};
void print_board(Board *board){
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            printf("%c ", piece_arr[board->mailbox[i * 8 + j]]);
        }printf("\n");
    }
}

void print_u64(uint64_t board){
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            uint64_t index = 8 * i + j;
            uint64_t res = 1;
            res = res << index;
            if(res & board){
                printf("1");
            }else{
                printf("0");
            }printf(" ");
        }printf("\n");
    }
}

uint64_t perft(Board *board, int depth){
    if(depth == 0)
        return 1;
    uint64_t nodes = 0;
    uint16_t move_list[256];
    int num_moves;
    num_moves = generate_moves(board, move_list);
    for(int i = 0; i < num_moves; i++){
        make_move(board, move_list[i]);
        if(!move_invalid(board, move_list[i])){
            nodes += perft(board, depth - 1);
        }
        unmake_move(board, move_list[i]);
    }return nodes;
}

int is_checkmate(Board *board){
    uint16_t move_list[256];
    int num_moves = generate_moves(board, move_list);
    for(int i = 0; i < num_moves; i++){
        make_move(board, move_list[i]);
        if(!move_invalid(board, move_list[i])){
            unmake_move(board, move_list[i]);
            return 0;
        }unmake_move(board, move_list[i]);
    }return 1;
}