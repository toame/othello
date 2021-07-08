#include "search.h"
#include <utility>
#include <bitset>
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
	const int param = 6;
	if (is_finished_game(state)) {
		return getResult(state) * 100;
	}
	int player = 0, opponent = 0;
	int u[4] = { 0, 7, 56, 63 };
	for (int i = 0; i < 4; i++) {
		if (state.getPlayerBoard() & (1ULL << u[i])) player++;
		if (state.getOpponentBoard() & (1ULL << u[i])) opponent++;
	}
	return legalMoveCounter(state) + (player - opponent) * param;
}

Operator Search::search(State state) {
	std::vector<Operator> moves = get_legal_move_vector(state);
	if (moves.size() == 0) return 0;
	Operator next_ope = 0;
	int max_eval = -10000000;
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

std::pair<int, Operator> Search::search2(State state, int depth) {
	if (depth == 0) return std::make_pair(Search::evaluate(state), 0);
	std::vector<Operator> moves = get_legal_move_vector(state);
	if (is_finished_game(state)) return std::make_pair(Search::evaluate(state), 0);
	Operator next_ope = 0;
	int max_eval = -10000000;
	if (moves.size() == 0) moves.push_back(0);
	for (auto ope : moves) {
		State next_state = makeMove(state, ope);
		int eval = -Search::search2(next_state, depth - 1).first;
		if (max_eval < eval) {
			max_eval = eval;
			next_ope = ope;
		}
	}
	return std::make_pair(max_eval, next_ope);
}
