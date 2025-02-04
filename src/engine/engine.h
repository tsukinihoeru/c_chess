#ifdef __cplusplus
extern "C" {
#endif

#include "../move_gen/bitboard.h"

class Engine{
public:
    Board *board;
    uint16_t root_search(int alpha, int beta, int depth);
    int search(int alpha, int beta, int depth);
};

#ifdef __cplusplus
} // closing brace for extern "C"
#endif