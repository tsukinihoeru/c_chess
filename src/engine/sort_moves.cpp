#include "engine.h"

const int mvv_lva [64] = {
    0, 0, 0, 0, 0, 0, 0, 0, //Padding
    0, 0, 0, 0, 0, 0, 0, 0, //Padding
    0, 0, 15, 14, 13, 12, 11, 10, // Victim P, attacker NULL NULL P B N R Q K
    0, 0, 25, 24, 23, 22, 21, 20, // Victim N, attacker NULL NULL P B N R Q K
    0, 0, 35, 34, 33, 32, 31, 10, // Victim B, attacker NULL NULL P B N R Q K
    0, 0, 45, 44, 43, 42, 41, 40, // Victim R, attacker NULL NULL P B N R Q K
    0, 0, 55, 54, 53, 52, 51, 50, // Victim Q, attacker NULL NULL P B N R Q K
    0, 0, 0, 0, 0, 0, 0, 0
};

inline int Engine::get_move_score(uint16_t move){
    int source = (move >> 10) & 0x3f;
    int dest = (move >> 4) & 0x3f;
    if(move & 0x04){
        int lookup_index = (board->mailbox[source] >> 1) + 8 * (board->mailbox[dest] >> 1);
        return mvv_lva[lookup_index];
    }return 0;
}

void Engine::sort_move(uint16_t *move_list, int start_ind, int num_moves){
    for(int i = start_ind; i < num_moves; i++){
        if(get_move_score(move_list[i]) > get_move_score(move_list[start_ind])){
            uint16_t temp = move_list[i];
            move_list[i] = move_list[start_ind];
            move_list[start_ind] = temp;
        }
    }
}