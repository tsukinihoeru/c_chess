#ifdef __cplusplus
extern "C" {
#endif

#include "../move_gen/bitboard.h"

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#include "transposition.h"

class Engine{
public:
    Board *board;
    Transposition_Table transposition_table;
    Engine() : transposition_table(128){};
    uint16_t iterative_deepening(int max_depth);
    uint16_t root_search(int alpha, int beta, int depth);
    int search(int alpha, int beta, int depth, int ply, bool is_pv);
    int quiesce(int alpha, int beta);
    inline int get_move_score(uint16_t move);
    void sort_move(uint16_t *move_list, int start_ind, int num_moves);
};

extern int last_search_score;