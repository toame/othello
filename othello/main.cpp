
#include <iostream>
#include <bitset>
//#include "state.h";
#include "search.h";
int main()
{
    Search search;
    int win = 0;
    int draw = 0;
    int lose = 0;
    for (int g = 0; g < 10000; g++) {
        State state;
        while (!is_finished_game(state)) {
            Operator ope;
            if (state.turn == (WHITE == g % 2)) {
                ope = search.make_random_next_move(state);
            }
            else {
                ope = search.search(state);
            }
            state = makeMove(state, ope);

            //std::cout << search.evaluate(state) << std::endl;
        }
        if ((getResult(state) > 0) ^ (WHITE == g % 2)) win++;
        else if ((getResult(state) < 0) ^ (WHITE == g % 2)) lose++;
        else draw++;
        if ((g + 1) % 1000 == 0) {
            std::cout << getString(state) << std::endl;
           
            std::cout << getResult(state) << " " << win << ":" << draw << ":" << lose << std::endl;
        }
    }

}
