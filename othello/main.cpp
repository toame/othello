
#include <iostream>
#include <bitset>
//#include "state.h";
#include "search.h";
int main()
{
    State state;
    Search search;
    std::cout << getString(state) << std::endl;

    state = makeMove(state, (1 << (8 * 3 + 2)));
    std::cout << getString(state) << std::endl;

    while (!is_finished_game(state)) {
        Operator ope = search.make_next_move(state);
        state = makeMove(state, ope);
        std::cout << getString(state) << std::endl;
    }

}
