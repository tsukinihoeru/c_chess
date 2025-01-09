#include "bitboard.h"

const int BB_SIZE = 8;
const int NUM_SQUARES = 64;
const int WHITE = 0;
const int BLACK = 1;
const int PAWN_BOARD = 2;
const int KNIGHT_BOARD = 3;
const int BISHOP_BOARD = 4;
const int ROOK_BOARD = 5;
const int QUEEN_BOARD = 6;
const int KING_BOARD = 7;

const int QUIET_FLAG = 0;
const int DOUBLE_PUSH_FLAG = 1;
const int KS_CASTLE_FLAG = 2;
const int QS_CASTLE_FLAG = 3;
const int CAPTURE_FLAG = 4;
const int EN_PASSANT_FLAG = 5;
const int KNIGHT_PROMO_FLAG = 8;
const int BISHOP_PROMO_FLAG = 9;
const int ROOK_PROMO_FLAG = 10;
const int QUEEN_PROMO_FLAG = 11;
const int KNIGHT_PROMO_CAP_FLAG = 12;
const int BISHOP_PROMO_CAP_FLAG = 13;
const int ROOK_PROMO_CAP_FLAG = 14;
const int QUEEN_PROMO_CAP_FLAG = 15;

const int WKS_CASTLING_RIGHTS = 8;
const int WQS_CASTLING_RIGHTS = 4;
const int BKS_CASTLING_RIGHTS = 2;
const int BQS_CASTLING_RIGHTS = 1;

void clear_board(Board *board) {
    for (int i = 0; i < BB_SIZE; i++) {
        board->bitboards[i] = 0;
    }
    for (int i = 0; i < NUM_SQUARES; i++) {
        board->mailbox[i] = 0;
    }board->ply = 0;
    board->history[0].castling_rights = 0;
    board->history[0].en_passant_square = 0;
}

//small helper function for parse_board
int fen_char_to_piece(char c){
    int color = BLACK;
    if(isupper(c))
        color = WHITE;
    
    c = toupper(c);

    switch (c){
        case 'P':
            return PAWN_BOARD * 2 + color;
        break;
        case 'N':
            return KNIGHT_BOARD * 2 + color;
        break;
        case 'B':
            return BISHOP_BOARD * 2 + color;
        break;
        case 'R':
            return ROOK_BOARD * 2 + color;
        break;
        case 'Q':
            return QUEEN_BOARD * 2 + color;
        break;
        case 'K':
            return KING_BOARD * 2 + color;
        break;
    }
    return -1;
}

//Errors with invalid fen, use separate function to validate fen
void parse_board(Board *board, char *fen) {
    clear_board(board);
    int rank = 7;
    int file = 0;
    while(*fen != EOF){
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
        }else if(*fen == 'K'){
            board->history[board->ply].castling_rights |= WKS_CASTLING_RIGHTS;
        }else if(*fen == 'Q'){
            board->history[board->ply].castling_rights |= WQS_CASTLING_RIGHTS;
        }else if(*fen == 'k'){
            board->history[board->ply].castling_rights |= BKS_CASTLING_RIGHTS;
        }else if(*fen == 'q'){
            board->history[board->ply].castling_rights |= BQS_CASTLING_RIGHTS;
        }
        fen++;
    }
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

void print_board(Board *board){
    for(int k = 0; k < 8; k++){
        print_u64(board->bitboards[k]);
        printf("\n");
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