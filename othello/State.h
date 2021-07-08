#include <iostream>
#include <string>
#define UPPER (0)
#define UPPER_RIGHT (1)
#define RIGHT (2)
#define LOWER_RIGHT (3)
#define LOWER (4)
#define LOWER_LEFT (5)
#define LEFT (6)
#define UPPER_LEFT (7)
#define BLACK (1)
#define WHITE (0)
#define BOARD_SIZE (8)
typedef unsigned long long int Uint64;

typedef struct State {
	Uint64 white, black;
	bool turn;
	State() {
		black = 0x0000000810000000;
		white = 0x0000001008000000;
		turn = BLACK;
	}

} State;

typedef unsigned long long int Operator;

std::string getString(State state) {
	std::string str;
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		int board_num = i;
		if (state.black & (1ULL << board_num)) {
			str += 'o';
		}
		else if (state.white & (1ULL << board_num)) {
			str += 'x';
		}
		else {
			str += '_';
		}
		if (i % 8 == 7)
			str += '\n';
	}
	return str;
}
Uint64 transfer(Operator ope, int dir) {
	if (dir == UPPER) return (ope << 8) & 0xffffffffffffff00;
	else if (dir == UPPER_RIGHT) return (ope << 7) & 0x7f7f7f7f7f7f7f00;
	else if (dir == RIGHT) return (ope >> 1) & 0x7f7f7f7f7f7f7f7f;
	else if (dir == LOWER_RIGHT) return (ope >> 9) & 0x007f7f7f7f7f7f7f;
	else if (dir == LOWER) return (ope >> 8) & 0x00ffffffffffffff;
	else if (dir == LOWER_LEFT) return (ope >> 7) & 0x00fefefefefefefe;
	else if (dir == LEFT) return (ope << 1) & 0xfefefefefefefefe;
	else if (dir == UPPER_LEFT) return (ope << 9) & 0xfefefefefefefe00;
	else return 0;
}


State reverse(State state, Operator ope) {
	Uint64 reverse_bit = 0;
	Uint64 opponentBoard;
	Uint64 playerBoard;
	if (state.turn == BLACK) {
		playerBoard = state.black;
		opponentBoard = state.white;
	}
	else {
		playerBoard = state.white;
		opponentBoard = state.black;
	}
	// 8方向それぞれに対して、ひっくり返すかの判定を行う
	for (int dir = 0; dir < 8; dir++) {
		Uint64 reverse_bit_ = 0;
		Uint64 mask = transfer(ope, dir);

		// ひっくり返す方向に相手の石がある時、その石のフラグを持つ
		while (mask != 0 && (mask & opponentBoard) != 0) {
			reverse_bit_ |= mask;
			mask = transfer(mask, dir);
		}
		// 自分の石で挟めるなら、その方向の石をひっくり返すビットを追加
		if ((mask & playerBoard) != 0) {
			reverse_bit |= reverse_bit_;
		}
	}
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		std::cout << ((reverse_bit & (1ULL << i)) != 0);
		if (i % 8 == 7) std::cout << std::endl;
	}
	playerBoard ^= ope | reverse_bit;
	opponentBoard ^= reverse_bit;
	if (state.turn == BLACK) {
		state.black = playerBoard;
		state.white = opponentBoard;
	}
	else {
		state.black = opponentBoard;
		state.white = playerBoard;
	}
	std::cout << getString(state) << std::endl;
	return state;
}



State makeMove(State state, Operator ope) {
	state.turn = !state.turn;
	if (ope == 0) return state;
	return reverse(state, ope);
}

