#include "state.h"
#include <vector>

struct Search{
    Operator make_random_next_move(const State state) const;
    int evaluate(const State state);

    Operator search(State state);
};