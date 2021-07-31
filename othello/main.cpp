
#include <iostream>
#include <bitset>
#include <chrono>
//#include "state.h";
#include "search.h";
int main()
{
    Search search;
    int win = 0;
    int draw = 0;
    int lose = 0;
    std::chrono::system_clock::time_point  start, end; // 型は auto で可
    start = std::chrono::system_clock::now(); // 計測開始時間


    for (int g = 0; g < 10000; g++) {
        State state;
        while (!is_finished_game(state)) {
            Operator ope;
            if (state.turn == (WHITE == g % 2)) {
                ope = search.make_random_next_move(state);
            }
            else {
                //ope = search.search(state);
                //ope = search.search2(state, 2).second;
                int remain = std::bitset<64>(~(state.white | state.black)).count();
                std::pair<int, Operator> tmp;
                int depth = 6;
                //if (remain <= 18) depth = 8;
                if (remain <= 12) depth = 12;
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
        end = std::chrono::system_clock::now();  // 計測終了時間
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //処理に要した時間をミリ秒に変換
        if ((g + 1) % 100 == 0) {
            std::cout << getString(state) << std::endl;
            //if(state.turn == BLACK) showBoard(makeConfirmBoard(state));
           
            std::cout << getResult(state) << " " << win << ":" << draw << ":" << lose << "  t = " << elapsed << std::endl;
            
        }
        
    }
    
    return 0;

}
