
#include <iostream>
#include "State.h";

int main()
{
    State state;
    std::cout << getString(state) << std::endl;

    state = makeMove(state, (1 << (8 * 3 + 2)));
    std::cout << getString(state) << std::endl;

}
