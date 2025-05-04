#include "move_gen/bitboard.h"
#include "graphics/graphics.h"
#include "engine/engine.h"


int main() {
    //init_graphics();
    //start_game_loop();
    start();
    /*
    init_magics();
    Engine engine;
    Board board;
    engine.board = &board;
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    parse_board(engine.board, fen);
    print_move(engine.root_search(-10000000, 10000000, 7));*/
    
}

/*
    Board board;
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    parse_board(&board, fen);
    init_magics();
    clock_t begin = clock();
    uint64_t nodes = perft(&board, 6);
    clock_t end = clock();
    double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("%llu\n", nodes);
    printf("Finished in %f", time_spent);
    return 0;
    
*/