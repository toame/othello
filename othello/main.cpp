
#include <iostream>
#include <bitset>
#include <chrono>
#include <Windows.h>
#include "search.h";

// 1手読み 952841:4118:43041 勝率96.02% (R+553)
// 2手読み 987986:996:11018 勝率99.27% (R+853)
// 3手読み 99525:14:461 勝率99.5% (R+931)
// 3手読み+終盤6手完全読み 99697:5:298 勝率99.7% (R+1015)


int depth;			     // 何手先まで読むかの設定
int endgame_depth; // 最終版の読み切り深さの設定
int main()
{
	Search search;
	State state;
	//ゲーム開始
	std::string S;
	std::cout << "input first turn (Black, White):" << std::endl;
	std::cin >> S;
	int g = -1;
	if (S == "Black") g = 0;
	else if(S == "White") g = 1;
	while (g == -1) {
		std::cout << "input first turn (Black, White):" << std::endl;
		std::cin >> S;
		if (S == "Black") g = 0;
		else if (S == "White") g = 1;
	}
	do {
		std::cout << "choose AI difficulty (weak, mediam, strong)" << std::endl;
		std::cin >> S;
		if (S == "weak") {
			depth = 1;
			endgame_depth = 1;
		}
		else if (S == "mediam") {
			depth = 3;
			endgame_depth = 3;
		}
		else if (S == "strong") {
			depth = 9;
			endgame_depth = 18;
		}
		else {
			depth = -1;
		}
	} while (depth == -1);
	
	
	//ゲームが終了するまで
	while (!is_finished_game(state)) {
		Operator ope;
		
		Uint64 LegalBoard = makeLegalBoard(state);//合法手生成
		// 手がない時はPASS
		if (LegalBoard == 0) {
			ope = 0;
		}
		// 手が1手の時はその手を自動で指す
		else if (std::bitset<64>(LegalBoard).count() == 1) {
			ope = LegalBoard;
		}
		// 黒のターン
		else if (state.turn == (BLACK ^ g)) {
			//ope = search.make_random_next_move(state);
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
			// 残り手数がendgame_depth手以下なら読み切る
			if (remain <= endgame_depth) tmp = search.search(state, endgame_depth);
			else tmp = search.search(state, depth);
			ope = tmp.second;
			system("cls");
			std::cout << "AI Move:" << ope2str(ope) << ", eval = " << tmp.first << std::endl;
			
		}
		state = makeMove(state, ope);

		
	}

	// ゲーム結果表示
	std::cout << getString(state) << std::endl;
	std::cout << "Black:" << std::bitset<64>(state.black).count() << " White:" << std::bitset<64>(state.white).count() << std::endl;
	if ((getResult(state) > 0) ^ g) {
		std::cout << "PLAYER_WIN" << std::endl;
	}
	else if ((getResult(state) < 0) ^ g) {
		std::cout << "PLAYER_LOSE" << std::endl;
	}
	else  {
		std::cout << "DRAW" << std::endl;
	}



	return 0;

}


