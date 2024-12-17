#include "bitboard.h"

void clear_board(Board *board) {
    for (int i = 0; i < BB_SIZE; i++) {
        board->bitboards[i] = 0;
    }
    for (int i = 0; i < NUM_SQUARES; i++) {
        board->mailbox[i] = 0;
    }
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
            file += *fen -'0';
        }else if(!((rank * 16 + file) & 0x88)){
            int piece = fen_char_to_piece(*fen);
            add_piece(board, piece, rank * 8 + file);
            file++;
        }
        fen++;
    }
}

void add_piece(Board *board, int piece, int square){
    board->bitboards[piece & 1] |= occupy_square[square];
    board->bitboards[piece >> 1] |= occupy_square[square];
    board->mailbox[square] = piece;
}

char print_piece_arr[16] = {'0', '1', '2', '3', 'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'};

void print_board(Board *board){
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            int piece = board->mailbox[i * 8 + j];
            if(piece){
                printf("%c", print_piece_arr[piece]);
            }else{
                printf(".");
            }
        }printf("\n");
    }
}