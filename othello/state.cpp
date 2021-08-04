#include <iostream>
#include <string>
#include <bitset>
#include "State.h"

std::string getString(const State state) {
	std::string str;
	str += "  ABCDEFGH\n";
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		int board_num = i;
		if (i % 8 == 0) {
			str += (char)('1' + (i / 8));
			str += " ";
		}
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
std::string ope2str(Uint64 ope) {
	std::string S;
	for (int i = 0; i < 64; i++) {
		if ((ope & (1ULL << i)) != 0) {
			S += (char)((i % 8) + 'A');
			S += (char)((i / 8) + '1');
			return S;
		}
	}
	return "PASS";
}
Uint64 transfer(const Operator ope, const int dir) {
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

// operatorに対応して石をひっくり返した盤面状態を返す
State reverse(State state, const Operator ope) {
	Uint64 reverse_bit = 0;
	Uint64 opponentBoard = state.getOpponentBoard();
	Uint64 playerBoard = state.getPlayerBoard();

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

	return state;
}


State makeMove(State state, const Operator ope) {
	State next_state = reverse(state, ope);
	next_state.turn = !next_state.turn;
	return next_state;
}

// 合法手のビットボードを生成
Uint64 makeLegalBoard(const State state) {
	const Uint64 horizontalBoard = state.getOpponentBoard() & 0x7e7e7e7e7e7e7e7e;
	const Uint64 verticalBoard = state.getOpponentBoard() & 0x00FFFFFFFFFFFF00;
	const Uint64 allSideBoard = state.getOpponentBoard() & 0x007e7e7e7e7e7e00;
	const Uint64 blankBoard = ~(state.getPlayerBoard() | state.getOpponentBoard());
	Uint64 tmp;
	Uint64 legalBoard;
	
	// 左
	tmp = horizontalBoard & (state.getPlayerBoard() << 1);
	for (int i = 0; i < 5; i++) {
		tmp |= horizontalBoard & (tmp << 1);
	}
	legalBoard = blankBoard & (tmp << 1);
	
	// 右
	tmp = horizontalBoard & (state.getPlayerBoard() >> 1);
	for (int i = 0; i < 5; i++) {
		tmp |= horizontalBoard & (tmp >> 1);
	}
	legalBoard |= blankBoard & (tmp >> 1);

	// 上
	tmp = verticalBoard & (state.getPlayerBoard() << 8);
	for (int i = 0; i < 5; i++) {
		tmp |= verticalBoard & (tmp << 8);
	}
	legalBoard |= blankBoard & (tmp << 8);

	// 下
	tmp = verticalBoard & (state.getPlayerBoard() >> 8);
	for (int i = 0; i < 5; i++) {
		tmp |= verticalBoard & (tmp >> 8);
	}
	legalBoard |= blankBoard & (tmp >> 8);

	// 右上
	tmp = allSideBoard & (state.getPlayerBoard() << 7);
	for (int i = 0; i < 5; i++) {
		tmp |= allSideBoard & (tmp << 7);
	}
	legalBoard |= blankBoard & (tmp << 7);

	// 左上
	tmp = allSideBoard & (state.getPlayerBoard() << 9);
	for (int i = 0; i < 5; i++) {
		tmp |= allSideBoard & (tmp << 9);
	}
	legalBoard |= blankBoard & (tmp << 9);

	// 右下
	tmp = allSideBoard & (state.getPlayerBoard() >> 9);
	for (int i = 0; i < 5; i++) {
		tmp |= allSideBoard & (tmp >> 9);
	}
	legalBoard |= blankBoard & (tmp >> 9);

	// 左下
	tmp = allSideBoard & (state.getPlayerBoard() >> 7);
	for (int i = 0; i < 5; i++) {
		tmp |= allSideBoard & (tmp >> 7);
	}
	legalBoard |= blankBoard & (tmp >> 7);


	return legalBoard;
}

// 確定石のビットボードを作成
Uint64 makeConfirmBoard(const State state) {
	
	const Uint64 PlayerBoard = state.getPlayerBoard() & 0x8100000000000081;
	Uint64 ConfirmBoard = PlayerBoard;
	if (ConfirmBoard == 0) return ConfirmBoard;
	

	// 上
	Uint64 tmp = PlayerBoard & 0x0000000000000081;
	for (int i = 0; i < 7; i++) {
		tmp |= (tmp << 8) & state.getPlayerBoard();
	}
	ConfirmBoard |= tmp;

	// 下
	tmp = PlayerBoard & 0x8100000000000000;
	for (int i = 0; i < 7; i++) {
		tmp |= (tmp >> 8) & state.getPlayerBoard();
	}
	ConfirmBoard |= tmp;

	// 右
	tmp = PlayerBoard & 0x8000000000000080;
	for (int i = 0; i < 7; i++) {
		tmp |= (tmp >> 1) & state.getPlayerBoard();
	}
	ConfirmBoard |= tmp;

	// 左
	tmp = PlayerBoard & 0x0100000000000001;
	for (int i = 0; i < 7; i++) {
		tmp |= (tmp << 1) & state.getPlayerBoard();
	}
	ConfirmBoard |= tmp;

	return ConfirmBoard;

}


// 盤面を表示する
void showBoard(const Uint64 board) {
	for (int i = 0; i < BOARD_SIZE2; i++) {
		std::cout << ((board & (1ULL << i)) != 0);
		if (i % BOARD_SIZE == BOARD_SIZE - 1) {
			std::cout << std::endl;
		}
	}
}

// 合法手の数を返す関数
int legalMoveCounter(const State state) {
	Uint64 board = makeLegalBoard(state);
	return std::bitset<64>(makeLegalBoard(state)).count();
}
// ゲーム終了判定を行う
bool is_finished_game(const State state) {
	// 空きマスがなければ終了
	if (std::bitset<64>(state.white | state.black).count() == 0) return true;
	
	// 手番側に合法手があればゲームは終了していない
	const int playerLegalMove = legalMoveCounter(state);
	if (playerLegalMove != 0) return false;

	// PASS後に合法手があるか判定する
	// 
	// PASSの操作(手番反転を行う)
	State state2 = state;
	state2.turn = !state2.turn;
	
	// 相手番側に合法手があればゲームは終了していない
	const int opponentLegalMove = legalMoveCounter(state2);
	if (opponentLegalMove != 0) return false;

	// 両者打つ場所がないので終了している
	return true;
}

// 合法手のoperatorをvectorに変換
std::vector<Operator> get_legal_move_vector(const State state) {
	std::vector<Operator> v;
	const Uint64 legalBoard = makeLegalBoard(state);
	for (int i = 0; i < 64; i++) {
		if (legalBoard & (1ULL << i)) {
			v.push_back(1ULL << i);
		}
	}
	return v;
}
int getResult(const State state) {
	const int blackScore = std::bitset<64>(state.black).count();
	const int whiteScore = std::bitset<64>(state.white).count();
	const int remain = std::bitset<64>(~(state.white | state.black)).count();
	if (blackScore == 0) return 64 + remain;
	if (whiteScore == 0) return 64 + remain;
	return blackScore - whiteScore;
}