#include "bitboard.h"

int main(){
    Board board;
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    printf("Hope this works.\n\n");
    parse_board(&board, fen);
    print_board(&board);
    return 0;
}