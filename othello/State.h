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
State makeMove(State state, Operator ope);
Uint64 transfer(Operator ope, int dir);
State reverse(State state, Operator ope);
std::string getString(State state);


