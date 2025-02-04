#include "engine.h"
#include "evaluate.h"
#include "iostream"

const int INF = 0xFFFFFF;

uint16_t Engine::root_search(int alpha, int beta, int depth){
    uint16_t move_list[216];
    int num_moves = generate_moves(board, move_list);
    uint16_t best_move = move_list[0];
    for(int i = 0; i < num_moves; i++){
        make_move(board, move_list[i]);
        if(move_invalid(board, move_list[i])){
            unmake_move(board, move_list[i]);
            continue;
        }
        int move_score = -search(-beta, -alpha, depth - 1);
        unmake_move(board, move_list[i]);
        if(move_score > alpha){
            best_move = move_list[i];
            alpha = move_score;
        }
        print_move(move_list[i]);
        std::cout << " Score: " << alpha << "\n";
    }
    return best_move;
}

int Engine::search(int alpha, int beta, int depth){
    if(depth == 0)
        return simple_evaluate(board);
    uint16_t move_list[216];
    int num_moves = generate_moves(board, move_list);
    for(int i = 0; i < num_moves; i++){
        make_move(board, move_list[i]);
        if(move_invalid(board, move_list[i])){
            unmake_move(board, move_list[i]);
            continue;
        }
        int move_score = -search(-beta, -alpha, depth - 1);
        unmake_move(board, move_list[i]);
        if(move_score >= beta)
            return beta;
        if(move_score > alpha)
            alpha = move_score;
    }return alpha;
}