#include "search.h"
#include <utility>
#include <bitset>
#include <algorithm>
unsigned int randInt() {
	static unsigned int tx = 223476789, ty = 362436069, tz = 521288629, tw = 88675123;
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

int Search::evaluate(const State state, const int w1, const int w2, const int w3, const int w4) {
 	if (is_finished_game(state)) {
		int remain = std::bitset<64>(~(state.white | state.black)).count();
		if (state.turn == BLACK)
			return getResult(state) * 100;
		else
			return -getResult(state) * 100;
	}
	int corners_player = 0, corners_opponent = 0;
	int able_corners_player = 0, able_corners_opponent = 0;
	const int u[4] = { 0, 7, 56, 63 };
	const Uint64 Legalboard = makeLegalBoard(state);
	const Uint64 Legalboard_opponent = makeLegalOpponentBoard(state);
	for (int i = 0; i < 4; i++) {
		if (state.getPlayerBoard() & (1ULL << u[i])) corners_player++;
		if (state.getOpponentBoard() & (1ULL << u[i])) corners_opponent++;
		if (Legalboard & (1ULL << u[i])) able_corners_player++;
		if (Legalboard_opponent & (1ULL << u[i])) able_corners_opponent++;
	}
	const int Confirm_stone_player = std::bitset<64>(makeConfirmBoard(state)).count();
	const int Confirm_stone_opponent = std::bitset<64>(makeConfirmOpponentBoard(state)).count();

	const int able_put_count_player = std::bitset<64>(Legalboard).count();
	const int able_put_count_opponent = std::bitset<64>(Legalboard_opponent).count();
	return
		(able_put_count_player - able_put_count_opponent) * w1
		+ (Confirm_stone_player - Confirm_stone_opponent) * w2
		+ (able_corners_player - able_corners_opponent) * w3
		+ (corners_player - corners_opponent) * w4
		;
}

//Operator Search::search(State state) {
//	std::vector<Operator> moves = get_legal_move_vector(state);
//	if (moves.size() == 0) return 0;
//	Operator next_ope = 0;
//	int max_eval = -10000000;
//	for (auto ope : moves) {
//		State next_state = makeMove(state, ope);
//		int eval = -Search::evaluate(next_state);
//		if (max_eval < eval) {
//			max_eval = eval;
//			next_ope = ope;
//		}
//	}
//	return next_ope;
//}
//
//std::pair<int, Operator> Search::search2(State state, int depth) {
//	if (depth == 0) return std::make_pair(Search::evaluate(state), 0);
//	std::vector<Operator> moves = get_legal_move_vector(state);
//	if (is_finished_game(state)) return std::make_pair(Search::evaluate(state), 0);
//	Operator next_ope = 0;
//	int max_eval = -10000000;
//	if (moves.size() == 0) moves.push_back(0);
//	for (auto ope : moves) {
//		State next_state = makeMove(state, ope);
//		int eval = -Search::search2(next_state, depth - 1).first;
//		if (max_eval < eval) {
//			max_eval = eval;
//			next_ope = ope;
//		}
//	}
//	return std::make_pair(max_eval, next_ope);
//}
const int order[64] = { 0, 7, 56, 63,
							1, 2, 3, 4, 5, 6,
							8, 10, 11, 12, 13, 15,
						16, 17, 18, 19, 20, 21, 22, 23,
						24, 25, 26, 27, 28, 29, 30, 31,
						32, 33, 34, 35, 36, 37, 38, 39,
						40, 41, 42, 43, 44, 45, 46, 47,
							48, 50, 51, 52, 53, 55,
							57, 58, 59, 60, 61, 62, 
						9, 14, 49, 54};
std::pair<int, Operator> Search::search3(State state, int depth, int alpha, int beta, const int w1, const int w2, const int w3, const int w4) {
	if (depth == 0) return std::make_pair(Search::evaluate(state, w1, w2, w3, w4), 0);
	if (is_finished_game(state)) return std::make_pair(Search::evaluate(state, w1, w2, w3, w4), 0);
	Operator next_ope = 0;
	Uint64 legalBoard = makeLegalBoard(state);
	int first_alpha = -120000000;
	std::vector<std::pair<int, Uint64>> v;
	//if (depth > 5) {
	//	for (int i = 0; i < BOARD_SIZE2; i++) {
	//		if (legalBoard != 0 && !(legalBoard & (1ULL << order[i]))) continue;
	//		Operator ope = (1ULL << order[i]);
	//		State next_state = makeMove(state, ope);
	//		v.push_back({ Search::evaluate(next_state), ope });
	//	}
	//	sort(v.begin(), v.end());
	//	for (auto p : v) {
	//		Operator ope = p.second;
	//		State next_state = makeMove(state, ope);
	//		const int next_alpha = -Search::search3(next_state, depth - 1, -beta, -alpha).first;
	//		if (first_alpha < next_alpha) {
	//			next_ope = ope;
	//			first_alpha = next_alpha;
	//		}
	//		alpha = std::max(alpha, first_alpha);
	//		if (alpha >= beta) {
	//			return std::make_pair(alpha, next_ope);
	//		}
	//	}

	//	return std::make_pair(alpha, next_ope);
	//}
	
	for (int i = 0; i < BOARD_SIZE2; i++) {
		if (legalBoard != 0 && !(legalBoard & (1ULL << order[i]))) continue;
		Operator ope = (1ULL << order[i]);
		//if (legalBoard != 0 && !(legalBoard & (1ULL << i))) continue;
		//Operator ope = (1ULL << i);
		if (legalBoard == 0) ope = 0;
		State next_state = makeMove(state, ope);
		const int next_alpha = -Search::search3(next_state, depth - 1, -beta, -alpha, w1, w2, w3, w4).first;
		if (first_alpha < next_alpha) {
			next_ope = ope;
			first_alpha = next_alpha;
		}
		alpha = std::max(alpha, first_alpha);
		if (alpha >= beta) {
			//std::cerr << alpha << " " << beta << std::endl;
			return std::make_pair(alpha, next_ope);
		}
		//if (depth == 2) {
		//	std::cerr << alpha << " " << beta << std::endl;
		//}
		if (legalBoard == 0) break;
	}
	return std::make_pair(alpha , next_ope);
}