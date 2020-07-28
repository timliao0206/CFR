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
    file = fopen("games/holdem.limit.2p.reverse_blinds.GAME", "r");

    const Game* game = readGame(file);
    srand(0);
    //calculateFiveCardBucket(10);

    State state;
    initState(game, 0 , &state);

    const NullActionAbstraction* abs = new NullActionAbstraction();

    size_t num_entries_per_bucket[MAX_ROUNDS] = { 0,0,0,0 };

    const BettingNode* root = initBettingTree(state, game, abs, num_entries_per_bucket);

    int num_bucket[4] = { 10,10,10,10 };
    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);
    calculateFiveCardBucket(10);

    

    
    

    //RealProbBucketing_train* RPB = new RealProbBucketing_train();
    VanillaCfr cfr(game, num_entries_per_bucket, card_abs);

    const int iteration = 100000;

    int start_time = time(0);

    for (int i = 0; i < iteration; i++) {
        cfr.doIteration(root,1);

        if (i == iteration/1000) {
            cfr.printRegretSum("RunData/Vanilla_Cfr_100_RegretSum_fixed_version");
            cfr.printStrategySum("RunData/Vanilla_Cfr_100_StrategySum_fixed_version");
        }
        else if (i == iteration/100) {
            cfr.printRegretSum("RunData/Vanilla_Cfr_1000_RegretSum_fixed_version");
            cfr.printStrategySum("RunData/Vanilla_Cfr_1000_StrategySum_fixed_version");
        }
        else if (i == iteration/10) {
            cfr.printRegretSum("RunData/Vanilla_Cfr_10000_RegretSum_fixed_version");
            cfr.printStrategySum("RunData/Vanilla_Cfr_10000_StrategySum_fixed_version");
        }

        if (_kbhit()) {
            char input = _getch();
            if (input == 'a') {
                int curr_time = time(0);
                long spent = curr_time - start_time;
                if (i == 0) {
                    std::cout << "0 % complete\ntime spent is uncertain.\n";
                }
                else {
                    long expect_time = (iteration - i) * spent / i;

                    std::cout << std::fixed << std::setprecision(2) << (double)i*100 / iteration << "% complete.\n";
                    std::cout << "(expected)will end in " << expect_time / 3600 << " hours " << (expect_time % 3600) / 60 << " minutes and " << expect_time % 60 << " second.\n";
                }
            }
        }
    }

    cfr.printRegretSum("RunData/Vanilla_Cfr_100000_RegretSum_fixed_version");
    cfr.printStrategySum("RunData/Vanilla_Cfr_100000_StrategySum_fixed_version");

    

    /*for (int i = 0; i < 4; i++) {
        cfr[i] = new VanillaCfr_RPB(game, num_entries_per_bucket, RPB);
    }*/


    /*ES* cfr[4];
    VanillaCfr* van[4];

    RandomPlayer* random_player = new RandomPlayer(game,card_abs);

    for (int i = 0; i < 4; i++) {
        cfr[i] = new ES(game, num_entries_per_bucket, card_abs);
        van[i] = new VanillaCfr(game, num_entries_per_bucket, card_abs);
    }

    cfr[0]->readFile("RunData/ES_1000_RegretSum");
    cfr[0]->readFile("RunData/ES_1000_StrategySum");
    cfr[1]->readFile("RunData/ES_10000_RegretSum");
    cfr[1]->readFile("RunData/ES_10000_StrategySum");
    cfr[2]->readFile("RunData/ES_100000_RegretSum");
    cfr[2]->readFile("RunData/ES_100000_StrategySum");
    cfr[3]->readFile("RunData/ES_1000000_RegretSum");
    cfr[3]->readFile("RunData/ES_1000000_StrategySum");

    van[0]->readFile("RunData/Vanilla_Cfr_10_RegretSum_WithFlopBucket");
    van[0]->readFile("RunData/Vanilla_Cfr_10_StrategySum_WithFlopBucket");
    van[1]->readFile("RunData/Vanilla_Cfr_100_RegretSum_WithFlopBucket");
    van[1]->readFile("RunData/Vanilla_Cfr_100_StrategySum_WithFlopBucket");
    van[2]->readFile("RunData/Vanilla_Cfr_1000_RegretSum_WithFlopBucket");
    van[2]->readFile("RunData/Vanilla_Cfr_1000_StrategySum_WithFlopBucket");
    van[3]->readFile("RunData/Vanilla_Cfr_10000_RegretSum_WithFlopBucket");
    van[3]->readFile("RunData/Vanilla_Cfr_10000_StrategySum_WithFlopBucket");

    for (int i = 0; i < 4; i++) {
        std::cout << "Vanilla Cfr with 10^" << i + 1 << " iteration:\n";
        for (int j = 0; j < 4; j++) {
            std::cout << "ES with 10^" << j + 3 << " iteration " << battle(game, root, van[i], cfr[j], 10000) << "\n";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < 4; i++) {
        std::cout << "Vanilla Cfr with 10^" << i + 1 << "iteration vs Random : " << battle(game, root, van[i], random_player, 10000) << "\n";
    }

    for (int i = 0; i < 4; i++) {
        std::cout << "ES with 10^" << i + 3 << "iteration vs Random : " << battle(game, root, cfr[i], random_player, 10000) << "\n";
    }*/

    
    /*std::cout << "1 vs 10 " << battle(game, root, cfr[0], cfr[1], 1000) << "\n";
    std::cout << "1 vs 100 " << battle(game, root, cfr[0], cfr[2], 1000) << "\n";
    std::cout << "1 vs 1000 " << battle(game, root, cfr[0], cfr[3], 1000) << "\n";
    std::cout << "\n";
    std::cout << "10 vs 100 " << battle(game, root, cfr[1], cfr[2], 1000) << "\n";
    std::cout << "10 vs 1000 " << battle(game, root, cfr[1], cfr[3], 1000) << "\n";
    std::cout << "\n";
    std::cout << "100 vs 1000 " << battle(game, root, cfr[2], cfr[3], 1000) << "\n";*/
    return 0;
}

