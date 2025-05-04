#include "engine.h"
#include "evaluate.h"
#include "iostream"

const int INF = 2000000;
const int CHECKMATE_SCORE = 1000000;
const int CONTEMPT = 0;
int last_search_score = 0;
const int aspiration_margin = 25;
void sort_tt_move(uint16_t *move_list, int num_moves, uint16_t best_move);

uint16_t Engine::iterative_deepening(int max_depth){
    uint16_t best_move = root_search(-2000000, 2000000, 3);
    for(int i = 4; i < max_depth; i++){
        int original_score = last_search_score;
        best_move = root_search(last_search_score - aspiration_margin, last_search_score + aspiration_margin, i);
        if(abs(original_score - last_search_score) >= aspiration_margin){
            best_move = root_search(-2000000, 2000000, i);
        }
    }return best_move;
}

uint16_t Engine::root_search(int alpha, int beta, int depth){
    uint16_t move_list[216];
    int num_moves = generate_moves(board, move_list);
    uint16_t best_move = move_list[0];
    uint16_t tt_move = 0;
    int tt_score = transposition_table.probe_value(board->zobrist_hash, depth, alpha, beta, &tt_move);
    if(tt_score != INVALID)
        return tt_score;
    for(int i = 0; i < num_moves; i++){
        if(tt_move && i == 0){
            sort_tt_move(move_list, num_moves, tt_move);
            best_move = tt_move;
        }else{
            sort_move(move_list, i, num_moves);
        }
        make_move(board, move_list[i]);
        if(move_invalid(board, move_list[i])){
            unmake_move(board, move_list[i]);
            continue;
        }
        int move_score = -search(-beta, -alpha, depth - 1, 1, true);
        unmake_move(board, move_list[i]);
        if(move_score > alpha){
            best_move = move_list[i];
            alpha = move_score;
        }
        //print_move(move_list[i]);
        //std::cout << " Score: " << alpha << "\n";
    }
    last_search_score = alpha;
    return best_move;
}

void sort_tt_move(uint16_t *move_list, int num_moves, uint16_t best_move){
    for(int i = 0; i < num_moves; i++){
        if(move_list[i] == best_move){
            move_list[i] = move_list[0];
            move_list[0] = best_move;
            return;
        }
    }
}

int Engine::search(int alpha, int beta, int depth, int ply, bool is_pv){
    if(depth == 0)
        return quiesce(alpha, beta);
    uint16_t move_list[216];
    int num_moves = generate_moves(board, move_list);
    int legal_moves = 0;
    //probe the hash table for the best move, and see if there's a valid score
    uint16_t tt_move = 0;
    uint16_t best_move = 0;
    int tt_score = transposition_table.probe_value(board->zobrist_hash, depth, alpha, beta, &tt_move);
    if(tt_score != INVALID)
        return tt_score;
    for(int i = 0; i < num_moves; i++){
        /*
            Sort the moves either by tt-move first or by move heuristics
        */
        if(tt_move && i == 0){
            sort_tt_move(move_list, num_moves, tt_move);
            best_move = tt_move;
        }else{
            sort_move(move_list, i, num_moves);
        }

        /*
            make the move, check if move is legal
        */
        make_move(board, move_list[i]);
        if(move_invalid(board, move_list[i])){
            unmake_move(board, move_list[i]);
            continue;
        }
        legal_moves++;

        /*
            PVS Search, where the recursion is done
            search non-pv moves with null window
        */
        int move_score;
        if(is_pv && i == 0){
             move_score = -search(-beta, -alpha, depth - 1, ply + 1, is_pv);
        }else{
            move_score = -search(-alpha - 1, -alpha, depth - 1, ply + 1, false);
            if(move_score > alpha && is_pv){
                move_score = -search(-beta, -alpha, depth - 1, ply + 1, is_pv);
            }
        }
        unmake_move(board, move_list[i]);

        /*
            update scores about the position
            check for beta cutoff and raise alpha
        */
        if(move_score >= beta){
            transposition_table.save_value(board->zobrist_hash, beta, depth, BETA_FLAG, move_list[i]);
            return beta;
        }
        if(move_score > alpha){
            best_move = move_list[i];
            alpha = move_score;
        }
    }
    //Mate/Stalemate detection
    //We decrease the value of checkmates by how far in the future they are
    //Therefore the engine prefers the fastest mate
    if(legal_moves == 0){
        //checkmate
        if(position_in_check(board))
            return -CHECKMATE_SCORE + ply;
        //stalemate
        else
            return CONTEMPT;
    }
    if(best_move)
        transposition_table.save_value(board->zobrist_hash, alpha, depth, EXACT_FLAG, best_move);
    return alpha;
}

int Engine::quiesce(int alpha, int beta){
    int static_score = simple_evaluate(board);
    if(static_score >= beta)
        return static_score;
    if(alpha < static_score)
        alpha = static_score;
    uint16_t move_list[216];
    int num_moves = generate_captures(board, move_list);
    for(int i = 0; i < num_moves; i++){
        sort_move(move_list, i, num_moves);
        make_move(board, move_list[i]);
        if(move_invalid(board, move_list[i])){
            unmake_move(board, move_list[i]);
            continue;
        }
        int move_score = -quiesce(-beta, -alpha);
        unmake_move(board, move_list[i]);
        if(move_score >= beta)
            return beta;
        if(move_score > alpha)
            alpha = move_score;
    }return alpha;
}