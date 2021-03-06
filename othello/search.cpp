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

// 合法手からランダムにoperatorを生成
Operator Search::make_random_next_move(const State state)const {
	Uint64 legalBoard = makeLegalBoard(state);
	std::vector<Operator> moves = get_legal_move_vector(state);
	if (moves.size() == 0) return 0;
	Operator next_ope = moves[randInt() % moves.size()];
	return next_ope;
}


// 盤面の状態から評価値を計算
int Search::evaluate(const State state) {
	const int w1 = 10, w2 = 23, w3 = 86, w4 = 37;
	// ゲームが終了していたら、石の差を評価の値とする
 	if (is_finished_game(state)) {
		int remain = std::bitset<64>(~(state.white | state.black)).count();
		if (state.turn == BLACK)
			return getResult(state) * 10000; // 石の差は最も信用されるべき評価なので、値を大きくする
		else
			return -getResult(state) * 10000;
	}
	int corners_player = 0, corners_opponent = 0;			//4隅の石の数
	int able_corners_player = 0, able_corners_opponent = 0;	//4隅に打てる数
	State opponent_state = state;
	opponent_state.turn = !opponent_state.turn;
	const int u[4] = { 0, 7, 56, 63 };
	const Uint64 Legalboard = makeLegalBoard(state);
	const Uint64 Legalboard_opponent = makeLegalBoard(opponent_state);
	// 端の状態を計算
	for (int i = 0; i < 4; i++) {
		if (state.getPlayerBoard() & (1ULL << u[i])) corners_player++;
		if (opponent_state.getPlayerBoard() & (1ULL << u[i])) corners_opponent++;
		if (Legalboard & (1ULL << u[i])) able_corners_player++;
		if (Legalboard_opponent & (1ULL << u[i])) able_corners_opponent++;
	}
	const int Confirm_stone_player = std::bitset<64>(makeConfirmBoard(state)).count();			// 手番側の確定石の数
	const int Confirm_stone_opponent = std::bitset<64>(makeConfirmBoard(opponent_state)).count();// 相手番側の確定石の数

	const int able_put_count_player = std::bitset<64>(Legalboard).count();						// 合法手の数
	const int able_put_count_opponent = std::bitset<64>(Legalboard_opponent).count();			// 手番反転の合法手の数
	return
		(able_put_count_player - able_put_count_opponent) * w1	// 打てる手の差を評価
		+ (Confirm_stone_player - Confirm_stone_opponent) * w2  // 確定石の差を評価
		+ (able_corners_player - able_corners_opponent) * w3	// 4隅を打てる手の差を評価
		+ (corners_player - corners_opponent) * w4				// 4隅の石の差を評価
		;
}
// 端の手から先に読むように設定する
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

// alpha-beta探索で行う
std::pair<int, Operator> Search::search(State state, int depth) {
	return search_(state, depth, -100000000, 100000000);
}

std::pair<int, Operator> Search::search_(State state, int depth, int alpha, int beta) {
	if (depth == 0) return std::make_pair(Search::evaluate(state), 0);				// 残り探索深さが0なら評価
	if (is_finished_game(state)) return std::make_pair(Search::evaluate(state), 0); // 盤面が終了していたら評価
	Operator next_ope = 0;
	Uint64 legalBoard = makeLegalBoard(state);
	int first_alpha = -100000000;
	std::vector<std::pair<int, Uint64>> v;

	// 合法手を探索する
	for (int i = 0; i < BOARD_SIZE2; i++) {
		if (legalBoard != 0 && !(legalBoard & (1ULL << order[i]))) continue;
		Operator ope = (1ULL << order[i]);
		if (legalBoard == 0) ope = PASS;
		State next_state = makeMove(state, ope);
		const int next_alpha = -Search::search_(next_state, depth - 1, -beta, -alpha).first;
		if (first_alpha < next_alpha) {
			next_ope = ope;
			first_alpha = next_alpha;
		}
		alpha = std::max(alpha, first_alpha);
		if (alpha >= beta) {
			return std::make_pair(alpha, next_ope);
		}
		if (legalBoard == 0) break;	// PASSを調べたら終了
	}
	return std::make_pair(alpha , next_ope);
}