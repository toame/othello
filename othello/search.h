#include "state.h"
#include <vector>

struct Search{
    Operator make_random_next_move(const State state) const;
    int evaluate(const State state);

    std::pair<int, Operator> search(State state, int depth);
    std::pair<int, Operator> search_(State state, int depth, int alpha, int beta);
};