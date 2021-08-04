
#include <iostream>
#include <bitset>
#include <chrono>
#include <Windows.h>
//#include "state.h";
#include "search.h";
unsigned int randInt2() {
	static unsigned int tx = 223476789, ty = 362436069, tz = 521288629, tw = 88675123;
	unsigned int tt = (tx ^ (tx << 11));
	tx = ty; ty = tz; tz = tw;
	return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}
// 1手読み 952841:4118:43041 勝率96.02% (R+553)
// 2手読み 987986:996:11018 勝率99.27% (R+853)
// 3手読み 99525:14:461 勝率99.5% (R+931)
// 3手読み+終盤6手完全読み 99525:14:461 勝率99.7% (R+1015)
const int opt_w1 = 10, opt_w2 = 23, opt_w3 = 86, opt_w4 = 37;
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
const int depth = 4;			     // 何手先まで読むかの設定
const int endgame_depth = depth * 2; // 最終版の読み切り深さの設定
int main()
{
	Search search;
	State state;
	
	int g = 0;
	while (!is_finished_game(state)) {
		Operator ope;
		
		Uint64 LegalBoard = makeLegalBoard(state);
		// 手がない時はPASS
		if (LegalBoard == 0) {
			ope = 0;
		}
		// 手が1手の時はその手を自動で指す
		else if (std::bitset<64>(~(state.white | state.black)).count() == 1) {
			ope = LegalBoard;
		}
		// 黒のターン
		else if (state.turn == BLACK) {
			std::cout << getString(state) << std::endl;
			std::cout << "Player Move:";
			std::string S;
			std::cin >> S;
			int num = 0;
			if (S == "RANDOM") {
				ope = search.make_random_next_move(state);
			}
			else {
				num = S[0] - 'A' + (S[1] - '1') * 8;
				ope = (1ULL << num);
			}
			// 入力が非合法手なら、再度入力を行ってもらう
			while ((ope & LegalBoard) == 0) {
				std::cout << S << " is None Move" << std::endl;
				std::cout << "Player_Move:";
				std::cin >> S;
				num = S[0] - 'A' + (S[1] - '1') * 8;
				ope = (1ULL << num);
			}
		}
		// 白のターン
		else {
			std::pair<int, Operator> tmp;
			int remain = std::bitset<64>(~(state.white | state.black)).count();
			// 残り手数が12手以下なら読み切る
			if (remain <= 12) tmp = search.search(state, endgame_depth, -100000000, 100000000, opt_w1, opt_w2, opt_w3, opt_w4);
			else tmp = search.search(state, depth, -100000000, 100000000, opt_w1, opt_w2, opt_w3, opt_w4);
			ope = tmp.second;
			system("cls");
			std::cout << "AI Move:" << ope2str(ope) << ", eval = " << tmp.first << std::endl;
			
		}
		state = makeMove(state, ope);

		
	}

	// ゲーム結果表示
	std::cout << getString(state) << std::endl;
	if (getResult(state) > 0) {
		std::cout << "PLAYER_WIN" << std::endl;
	}
	else if (getResult(state) < 0) {
		std::cout << "PLAYER_LOSE" << std::endl;
	}
	else  {
		std::cout << "DRAW" << std::endl;
	}



	return 0;

}


