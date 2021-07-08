#include "search.h"
unsigned int randInt() {
	static unsigned int tx = 123476789, ty = 362436069, tz = 521288629, tw = 88675123;
	unsigned int tt = (tx ^ (tx << 11));
	tx = ty; ty = tz; tz = tw;
	return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}

Operator Search::make_random_next_move(const State state)const {
	Uint64 legalBoard = makeLegalBoard(state);
	std::vector<Operator> moves = get_legal_move_vector(state);
	if (moves.size() == 0) return 0;
	Operator next_ope = moves[randInt() % moves.size()];
	return next_ope;
}

int Search::evaluate(const State state) {
	return legalMoveCounter(state);
}

Operator Search::search(State state) {
	std::vector<Operator> moves = get_legal_move_vector(state);
	if (moves.size() == 0) return 0;
	Operator next_ope = 0;
	int max_eval = -256;
	for (auto ope : moves) {
		State next_state = makeMove(state, ope);
		int eval = -Search::evaluate(next_state);
		if (max_eval < eval) {
			max_eval = eval;
			next_ope = ope;
		}
	}
	return next_ope;
}

