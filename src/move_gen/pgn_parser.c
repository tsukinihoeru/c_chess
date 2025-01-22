#include "bitboard.h"

const char pgn_piece_convert[8] = {'0', '1', '2', 'N', 'B', 'R', 'Q', 'K'};
//returns length of string, stores pgn string in char*pgn
//called BEFORE the move is made
//missing disambiguation, so annoying to implement
void get_move_pgn(Board *board, uint16_t move, char * pgn){
    int source = (move >> 10) & 0x3f;
    int dest = (move >> 4) & 0x3f;
    int flag = move & 0x0f;
    if(flag == KS_CASTLE_FLAG){
        pgn[0] = 'O'; pgn[1] = '-'; pgn[2] = 'O'; pgn[3] = '\0';
        return;
    }
    if(flag == QS_CASTLE_FLAG){
        pgn[0] = 'O'; pgn[1] = '-'; pgn[2] = 'O'; pgn[3] = '-'; pgn[4] = 'O'; pgn[5] = '\0';
        return;
    }
    int piece = board->mailbox[source] / 2;
    if(flag == DOUBLE_PUSH_FLAG || (flag == QUIET_FLAG && piece == PAWN_BOARD)){
        *pgn++ = dest % 8 + 'a';
        *pgn++ = dest / 8 + '1';
        *pgn++ = '\0';
        return;
    }
    if(piece != PAWN_BOARD)
        *pgn++ = pgn_piece_convert[piece];
    else
        *pgn++ = source % 8 + 'a';
    if(flag & 0b0100){
        *pgn++ = 'x';
    }
    *pgn++ = dest % 8 + 'a';
    *pgn++ = dest / 8 + '1';
    //add in check and mate detection here
    make_move(board, move);
    if(position_in_check(board)){
        if(is_checkmate(board))
            *pgn++ = '#';
        else
            *pgn++ = '+';
    }
    unmake_move(board, move);
    *pgn++ = '\0';
}