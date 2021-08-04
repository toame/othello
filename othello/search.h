#include "state.h"
#include <vector>

struct Search{
    Operator make_random_next_move(const State state) const;
    int evaluate(const State state, const int w1, const int w2, const int w3, const int w4);

    std::pair<int, Operator> search(State state, int depth, int alpha, int beta, const int w1, const int w2, const int w3, const int w4);
};