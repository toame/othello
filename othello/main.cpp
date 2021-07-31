
#include <iostream>
#include <bitset>
#include <chrono>
//#include "state.h";
#include "search.h";
unsigned int randInt2() {
    static unsigned int tx = 123476789, ty = 362436069, tz = 521288629, tw = 88675123;
    unsigned int tt = (tx ^ (tx << 11));
    tx = ty; ty = tz; tz = tw;
    return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}
// 1手読み 952841:4118:43041 勝率96.02% (R+553)
// 2手読み 987986:996:11018 勝率99.27% (R+853)
// 3手読み 99525:14:461 勝率99.5% (R+931)
// 3手読み+終盤6手完全読み 99525:14:461 勝率99.7% (R+1015)


int main()
{
    Search search;
    
    std::chrono::system_clock::time_point  start, end; // 型は auto で可
    start = std::chrono::system_clock::now(); // 計測開始時間
    //(depth, w1, w2, w3, w4) = (2, 10, 21, 88, 52)     => 99248:23:729
    //(depth, w1, w2, w3, w4) = (2, 10, 20, 100, 100)   => 98987:17:996
    
    int opt_w1 = 10, opt_w2 = 23, opt_w3 = 86, opt_w4 = 37;
    while(true) {
        int win = 0;
        int draw = 0;
        int lose = 0;
        //int w1, w2, w3, w4;
        for (int g = 0; g < 10000; g++) {
            State state;
            while (!is_finished_game(state)) {
                Operator ope;
                int remain = std::bitset<64>(~(state.white | state.black)).count();
                if (remain > 150) {
                    ope = search.make_random_next_move(state);
                }
                else if (state.turn == (WHITE == g % 2)) {
                    ope = search.make_random_next_move(state);
                    //std::pair<int, Operator> tmp;
                    //int depth = 2;
                    ////if (remain <= 18) depth = 8;
                    //if (remain <= 4) depth = 4;
                    //tmp = search.search3(state, depth, -100000000, 100000000, w1, w2, w3, w4);
                    //ope = tmp.second;
                }
                else {

                    std::pair<int, Operator> tmp;
                    int depth = 6;
                    //if (remain <= 18) depth = 8;
                    if (remain <= 12) depth = 12;
                    tmp = search.search3(state, depth, -100000000, 100000000, opt_w1, opt_w2, opt_w3, opt_w4);
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
        //  return 0;
        //std::cout << "prev:" << opt_w1 << " " << opt_w2 << " " << opt_w3 << " " << opt_w4 << " win:" << win << std::endl;
        //std::cout << "next:" << w1 << " " << w2 << " " << w3 << " " << w4 << " win:" << lose << std::endl;
        //if (win + 20 < lose) {
        //    opt_w1 = w1;
        //    opt_w2 = w2;
        //    opt_w3 = w3;
        //    opt_w4 = w4;

        //}
        return 0;
    }
    
    return 0;

}
