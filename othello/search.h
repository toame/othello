#include "state.h"
#include <vector>
unsigned int randInt() {
    static unsigned int tx = 123476789, ty = 362436069, tz = 521288629, tw = 88675123;
    unsigned int tt = (tx ^ (tx << 11));
    tx = ty; ty = tz; tz = tw;
    return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}
struct Search{

	Operator make_next_move(State state) const {
        Uint64 legalBoard = makeLegalBoard(state);
        std::vector<int> v;
        for (int i = 0; i < 64; i++) {
            if (legalBoard & (1ULL << i)) {
                v.push_back(i);
            }
        }
        if (v.size() == 0) return 0;
        int next_move = v[randInt() % v.size()];
        return (1ULL << next_move);
	}
};