#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "betting_node.h"
#include <fstream>
#include <cassert>
#include <stack>
#include <time.h>
#include <chrono>
#include <conio.h>
#include <iomanip>
#include "CFR_family.h"
#include "evalHandTable.h"
#include <ctime>
#include "FiveCardBucket.h"

int main()
{
    FILE* file;
    file = fopen("games/holdem.nolimit.2p.reverse_blinds.GAME", "r");

    const Game* game = readGame(file);
    srand(0);
    //calculateFiveCardBucket(10);

    State state;
    initState(game, 0, &state);

    const NullActionAbstraction* abs = new NullActionAbstraction();

    std::array<int, MAX_ROUNDS> raise_time = { 3,3,3,3 };

    const PropotionalActionAbstraction* pabs = new PropotionalActionAbstraction(raise_time, 3);

    Action actions[11];

    int num_action = pabs->getActions(game, state, actions);


    size_t num_entries_per_bucket[MAX_ROUNDS] = { 0,0,0,0 };

    BettingNode::size = 0;

    const BettingNode* root = initBettingTree(state, game, pabs, num_entries_per_bucket,0);

    std::cout << BettingNode::size;
    /*
    int num_bucket[4] = { 10,10,10,10 };
    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);
    calculateFiveCardBucket(10);*/






    /*ES cfr(game,num_entries_per_bucket,card_abs);

    const long long iteration = 1e7;

    int start_time = time(0);

    std::string head = "RunData/ES_";
    std::string bottom = "000000_RegretSum";
    std::string bottom_strategy = "000000_StrategySum";

    for (long long i = 0; i < iteration; i++) {
        cfr.doIteration(root,1);

        if (i == iteration/1000) {
            cfr.printRegretSum("RunData/ES_10000_RegretSum");
            cfr.printStrategySum("RunData/ES_10000_StrategySum");
        }
        else if (i == iteration/100) {
            cfr.printRegretSum("RunData/ES_100000_RegretSum");
            cfr.printStrategySum("RunData/ES_100000_StrategySum");
        }

        if (i % (iteration / 10) == 0 && i != 0) {
            cfr.printRegretSum(head + std::to_string(i/(iteration/10)) + bottom);
            cfr.printStrategySum(head + std::to_string(i / (iteration / 10)) + bottom_strategy);
        }



        if (_kbhit()) {
            char input = _getch();
            if (input == 'a') {
                int curr_time = time(0);
                long long spent = curr_time - start_time;
                if (i == 0) {
                    std::cout << "0 % complete\ntime spent is uncertain.\n";
                }
                else {
                    long long expect_time = (iteration - i) * spent / i;

                    std::cout << std::fixed << std::setprecision(2) << (double)i*100 / iteration << "% complete.\n";
                    std::cout << "(expected)will end in " << expect_time / 3600 << " hours " << (expect_time % 3600) / 60 << " minutes and " << expect_time % 60 << " second.\n";
                }
            }
        }
    }

    cfr.printRegretSum("RunData/ES_10000000_RegretSum");
    cfr.printStrategySum("RunData/ES_1000000_StrategySum");*/



    /*for (int i = 0; i < 4; i++) {
        cfr[i] = new VanillaCfr_RPB(game, num_entries_per_bucket, RPB);
    }*/

    /*std::fstream battle_file;
    battle_file.open("RunData/battle_ES_7_linear",std::ios::out);


    ES* cfr[10];


    RandomPlayer* random_player = new RandomPlayer(game,card_abs);

    for (int i = 0; i < 10; i++) {
        cfr[i] = new ES(game, num_entries_per_bucket, card_abs);
        //os[i] = new OS(game, num_entries_per_bucket, card_abs);
    }

    for (int i = 1; i <= 10; i++) {
        cfr[i-1]->readFile("RunData/ES_" + std::to_string(i) + "000000_RegretSum");
    }*/

    /*os[0]->readFile("RunData/OS_1000_RegretSum_fixed_version");
    os[0]->readFile("RunData/OS_1000_StrategySum_fixed_version");
    os[1]->readFile("RunData/OS_10000_RegretSum_fixed_version");
    os[1]->readFile("RunData/OS_10000_StrategySum_fixed_version");
    os[2]->readFile("RunData/OS_100000_RegretSum_fixed_version");
    os[2]->readFile("RunData/OS_100000_StrategySum_fixed_version");
    os[3]->readFile("RunData/OS_1000000_RegretSum_fixed_version");
    os[3]->readFile("RunData/OS_1000000_StrategySum_fixed_version");

    for (int i = 0; i < 9; i++) {
        battle_file << "ES with" << i+1 <<"X10^6" << " iteration:\n";
        for (int j = i+1; j <= 9; j++) {
            battle_file << "ES with " << j + 1 << "X10^6 iteration " << battle(game, root, cfr[i], cfr[j], 1000) << "\n";
        }
        battle_file << "\n";
    }*/
    /*
    for (int i = 0; i < 4; i++) {
        std::cout << "Vanilla Cfr with 10^" << i + 1 << "iteration vs Random : " << battle(game, root, van[i], random_player, 10000) << "\n";
    }

    for (int i = 0; i < 4; i++) {
        std::cout << "ES with 10^" << i + 3 << "iteration vs Random : " << battle(game, root, cfr[i], random_player, 10000) << "\n";
    }*/
}
