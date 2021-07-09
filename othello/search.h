#include "state.h"
#include <vector>

struct Search{
    Operator make_random_next_move(const State state) const;
    int evaluate(const State state);

    Operator search(State state);
    std::pair<int, Operator> search2(State state, int depth);
    std::pair<int, Operator> search3(State state, int depth, int alpha, int beta);
};