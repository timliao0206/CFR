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
    calculateFiveCardBucket(10);

    State state;
    initState(game, 0 , &state);

    const NullActionAbstraction* abs = new NullActionAbstraction();

    size_t num_entries_per_bucket[MAX_ROUNDS] = { 0,0,0,0 };

    const BettingNode* root = initBettingTree(state, game, abs, num_entries_per_bucket);

    int num_bucket[4] = { 10,10,10,10 };
    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);

    /*ES cfr(game, num_entries_per_bucket, card_abs);

    const int printFreq = 1;

    int start_time = time(0);

    for (int i = 0; i < 1000000; i++) {
        cfr.doIteration(root,1);

        if (i == 1000) {
            cfr.printRegretSum("RunData/ES_1000_RegretSum_WithFlopBucket");
            cfr.printStrategySum("RunData/ES_1000_StrategySum_WithFlopBucket");
        }
        else if (i == 10000) {
            cfr.printRegretSum("RunData/ES_10000_RegretSum_WithFlopBucket");
            cfr.printStrategySum("RunData/ES_10000_StrategySum_WithFlopBucket");
        }
        else if (i == 100000) {
            cfr.printRegretSum("RunData/ES_100000_RegretSum_WithFlopBucket");
            cfr.printStrategySum("RunData/ES_100000_StrategySum_WithFlopBucket");
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
                    long expect_time = (1000000 - i) * spent / i;

                    std::cout << std::fixed << std::setprecision(2) << (double)i / 10000 << "% complete.\n";
                    std::cout << "(expected)will end in " << expect_time / 3600 << " hours " << (expect_time % 3600) / 60 << " minutes and " << expect_time % 60 << " second.\n";
                }
            }
        }
    }

    cfr.printRegretSum("RunData/ES_1000000_RegretSum_WithFlopBucket");
    cfr.printStrategySum("RunData/ES_1000000_StrategySum_WithFlopBucket");*/

    double battle_array[4][4];

    ES* cfr[4];

    VanillaCfr* van[4];

    RandomPlayer* random_player = new RandomPlayer(game,card_abs);

    for (int i = 0; i < 4; i++) {
        cfr[i] = new ES(game, num_entries_per_bucket, card_abs);
        van[i] = new VanillaCfr(game, num_entries_per_bucket, card_abs);
    }

    cfr[0]->readFile("RunData/ES_1000_RegretSum_WithFlopBucket");
    cfr[0]->readFile("RunData/ES_1000_StrategySum_WithFlopBucket");
    cfr[1]->readFile("RunData/ES_10000_RegretSum_WithFlopBucket");
    cfr[1]->readFile("RunData/ES_10000_StrategySum_WithFlopBucket");
    cfr[2]->readFile("RunData/ES_100000_RegretSum_WithFlopBucket");
    cfr[2]->readFile("RunData/ES_100000_StrategySum_WithFlopBucket");
    cfr[3]->readFile("RunData/ES_1000000_RegretSum_WithFlopBucket");
    cfr[3]->readFile("RunData/ES_1000000_StrategySum_WithFlopBucket");

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
            std::cout << "ES with 10^" << j + 3 << " iteration " << battle(game, root, van[i], cfr[j], 1000) << "\n";
        }
        std::cout << "\n";
    }

    /*
    std::cout << "10^3 vs 10^4" << battle(game, root, cfr[0], cfr[1], 1000) << "\n";
    std::cout << "10^3 vs 10^5" << battle(game, root, cfr[0], cfr[2], 1000) << "\n";
    std::cout << "10^3 vs 10^6" << battle(game, root, cfr[0], cfr[3], 1000) << "\n";
    std::cout << "\n";
    std::cout << "10^4 vs 10^5" << battle(game, root, cfr[1], cfr[2], 1000) << "\n";
    std::cout << "10^4 vs 10^6" << battle(game, root, cfr[1], cfr[3], 1000) << "\n";
    std::cout << "\n";
    std::cout << "10^5 vs 10^6" << battle(game, root, cfr[2], cfr[3], 1000) << "\n";*/
    return 0;
}

