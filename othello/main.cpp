
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
    for (int g = 0; g < 100; g++) {
        State state;
        while (!is_finished_game(state)) {
            Operator ope;
            if (state.turn == (WHITE == g % 2)) {
                ope = search.make_random_next_move(state);
            }
            else {
                //ope = search.search(state);
                //ope = search.search2(state, 2).second;
                //int remain = std::bitset<64>(~(state.white | state.black)).count();
                std::pair<int, Operator> tmp;
                const int depth = 6;
                //if (remain <= 18) depth = 8;
                //if (remain <= 14) depth = 12;
                tmp = search.search3(state, depth, -100000000, 100000000);
                ope = tmp.second;
                //std::cout << remain << " " << tmp.first << std::endl;
            }
            state = makeMove(state, ope);

            //std::cout << getString(state) << std::endl;
        }
        if ((getResult(state) > 0) ^ (WHITE == g % 2)) win++;
        else if ((getResult(state) < 0) ^ (WHITE == g % 2)) lose++;
        else draw++;
        if ((g + 1) % 10 == 0) {
            std::cout << getString(state) << std::endl;
           
            std::cout << getResult(state) << " " << win << ":" << draw << ":" << lose << std::endl;
        }
    }
    return 0;

}
