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
#define BOARD_SIZE2 (BOARD_SIZE * BOARD_SIZE)
typedef unsigned long long int Uint64;

typedef struct State {
	Uint64 white, black;
	bool turn;
	State() {
		black = 0x0000000810000000;
		white = 0x0000001008000000;
		turn = BLACK;
	}
	Uint64 getPlayerBoard() const {
		if (turn == BLACK) return black;
		else return white;
	}
	Uint64 getOpponentBoard() const {
		if (turn == BLACK) return white;
		else return black;
	}
} State;

typedef unsigned long long int Operator;
State makeMove(const State state, const Operator ope);
Uint64 transfer(const Operator ope, const int dir);
State reverse(State state, const Operator ope);
Uint64 makeLegalBoard(const State state);
std::string getString(const State state);
void showBoard(const Uint64 board);
int legalMoveCounter(const State state);
bool is_finished_game(State state);