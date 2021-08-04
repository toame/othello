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

// ‡–@è‚©‚çƒ‰ƒ“ƒ_ƒ€‚Éoperator‚ğ¶¬
Operator Search::make_random_next_move(const State state)const {
	Uint64 legalBoard = makeLegalBoard(state);
	std::vector<Operator> moves = get_legal_move_vector(state);
	if (moves.size() == 0) return 0;
	Operator next_ope = moves[randInt() % moves.size()];
	return next_ope;
}


// ”Õ–Ê‚Ìó‘Ô‚©‚ç•]‰¿’l‚ğŒvZ
int Search::evaluate(const State state, const int w1, const int w2, const int w3, const int w4) {
	// ƒQ[ƒ€‚ªI—¹‚µ‚Ä‚¢‚½‚çAÎ‚Ì·‚ğ•]‰¿‚Ì’l‚Æ‚·‚é
 	if (is_finished_game(state)) {
		int remain = std::bitset<64>(~(state.white | state.black)).count();
		if (state.turn == BLACK)
			return getResult(state) * 10000; // Î‚Ì·‚ÍÅ‚àM—p‚³‚ê‚é‚×‚«•]‰¿‚È‚Ì‚ÅA’l‚ğ‘å‚«‚­‚·‚é
		else
			return -getResult(state) * 10000;
	}
	int corners_player = 0, corners_opponent = 0;			//4‹÷‚ÌÎ‚Ì”
	int able_corners_player = 0, able_corners_opponent = 0;	//4‹÷‚É‘Å‚Ä‚é”
	State opponent_state = state;
	opponent_state.turn = !opponent_state.turn;
	const int u[4] = { 0, 7, 56, 63 };
	const Uint64 Legalboard = makeLegalBoard(state);
	const Uint64 Legalboard_opponent = makeLegalBoard(opponent_state);
	// ’[‚Ìó‘Ô‚ğŒvZ
	for (int i = 0; i < 4; i++) {
		if (state.getPlayerBoard() & (1ULL << u[i])) corners_player++;
		if (opponent_state.getPlayerBoard() & (1ULL << u[i])) corners_opponent++;
		if (Legalboard & (1ULL << u[i])) able_corners_player++;
		if (Legalboard_opponent & (1ULL << u[i])) able_corners_opponent++;
	}
	const int Confirm_stone_player = std::bitset<64>(makeConfirmBoard(state)).count();			// è”Ô‘¤‚ÌŠm’èÎ‚Ì”
	const int Confirm_stone_opponent = std::bitset<64>(makeConfirmBoard(opponent_state)).count();// ‘Šè”Ô‘¤‚ÌŠm’èÎ‚Ì”

	const int able_put_count_player = std::bitset<64>(Legalboard).count();						// ‡–@è‚Ì”
	const int able_put_count_opponent = std::bitset<64>(Legalboard_opponent).count();			// è”Ô”½“]‚Ì‡–@è‚Ì”
	return
		(able_put_count_player - able_put_count_opponent) * w1	// ‘Å‚Ä‚éè‚Ì·‚ğ•]‰¿
		+ (Confirm_stone_player - Confirm_stone_opponent) * w2  // Šm’èÎ‚Ì·‚ğ•]‰¿
		+ (able_corners_player - able_corners_opponent) * w3	// 4‹÷‚ğ‘Å‚Ä‚éè‚Ì·‚ğ•]‰¿
		+ (corners_player - corners_opponent) * w4				// 4‹÷‚ÌÎ‚Ì·‚ğ•]‰¿
		;
}
// ’[‚Ìè‚©‚çæ‚É“Ç‚Ş‚æ‚¤‚Éİ’è‚·‚é
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

// alpha-beta’Tõ‚Ås‚¤
std::pair<int, Operator> Search::search(State state, int depth, int alpha, int beta, const int w1, const int w2, const int w3, const int w4) {
	if (depth == 0) return std::make_pair(Search::evaluate(state, w1, w2, w3, w4), 0);				// c‚è’Tõ[‚³‚ª0‚È‚ç•]‰¿
	if (is_finished_game(state)) return std::make_pair(Search::evaluate(state, w1, w2, w3, w4), 0); // ”Õ–Ê‚ªI—¹‚µ‚Ä‚¢‚½‚ç•]‰¿
	Operator next_ope = 0;
	Uint64 legalBoard = makeLegalBoard(state);
	int first_alpha = -100000000;
	std::vector<std::pair<int, Uint64>> v;

	// ‡–@è‚ğ’Tõ‚·‚é
	for (int i = 0; i < BOARD_SIZE2; i++) {
		if (legalBoard != 0 && !(legalBoard & (1ULL << order[i]))) continue;
		Operator ope = (1ULL << order[i]);
		if (legalBoard == 0) ope = PASS;
		State next_state = makeMove(state, ope);
		const int next_alpha = -Search::search(next_state, depth - 1, -beta, -alpha, w1, w2, w3, w4).first;
		if (first_alpha < next_alpha) {
			next_ope = ope;
			first_alpha = next_alpha;
		}
		alpha = std::max(alpha, first_alpha);
		if (alpha >= beta) {
			return std::make_pair(alpha, next_ope);
		}
		if (legalBoard == 0) break;	// PASS‚ğ’²‚×‚½‚çI—¹
	}
	return std::make_pair(alpha , next_ope);
}