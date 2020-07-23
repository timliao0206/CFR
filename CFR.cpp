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

    State state;
    initState(game, 0 , &state);

    const NullActionAbstraction* abs = new NullActionAbstraction();

    size_t num_entries_per_bucket[MAX_ROUNDS] = { 0,0,0,0 };

    const BettingNode* root = initBettingTree(state, game, abs, num_entries_per_bucket);

    int num_bucket[4] = { 10,10,10,10 };
    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);

    VanillaCfr cfr(game, num_entries_per_bucket, card_abs);

    const int printFreq = 1;

    int start_time = time(0);

    for (int i = 0; i < 10000; i++) {
        cfr.doIteration(root,1);

        if (i == 10) {
            cfr.printRegretSum("RunData/Vanilla_Cfr_10_RegretSum_WithFlopBucket");
            cfr.printStrategySum("RunData/Vanilla_Cfr_10_StrategySum_WithFlopBucket");
        }
        else if (i == 100) {
            cfr.printRegretSum("RunData/Vanilla_Cfr_100_RegretSum_WithFlopBucket");
            cfr.printStrategySum("RunData/Vanilla_Cfr_100_StrategySum_WithFlopBucket");
        }
        else if (i == 1000) {
            cfr.printRegretSum("RunData/Vanilla_Cfr_1000_RegretSum_WithFlopBucket");
            cfr.printStrategySum("RunData/Vanilla_Cfr_1000_StrategySum_WithFlopBucket");
        }

        if (_kbhit()) {
            char input = _getch();
            if (input == 'a') {
                int curr_time = time(0);
                long spent = curr_time - start_time;
                long expect_time = (10000 - i) * spent / i;

                std::cout << std::fixed << std::setprecision(2) << (double)i / 100 << "% complete.\n";
                std::cout << "(expected)will end in " << expect_time / 3600 << " hours " << (expect_time % 3600) / 60 << " minutes and " << expect_time % 60 << " second.\n";
            }
        }
    }

    cfr.printRegretSum("RunData/Vanilla_Cfr_10000_RegretSum_WithFlopBucket");
    cfr.printStrategySum("RunData/Vanilla_Cfr_10000_StrategySum_WithFlopBucket");

    /*double battle_array[4][4];

    VanillaCfr* cfr[4];

    for (int i = 0; i < 4; i++) {
        cfr[i] = new VanillaCfr(game, num_entries_per_bucket, card_abs);
    }

    cfr[0]->readFile("Vanilla_Cfr_10_RegretSum");
    cfr[0]->readFile("Vanilla_Cfr_10_StrategySum");
    cfr[1]->readFile("Vanilla_Cfr_100_RegretSum");
    cfr[1]->readFile("Vanilla_Cfr_100_StrategySum");
    cfr[2]->readFile("Vanilla_Cfr_1000_RegretSum");
    cfr[2]->readFile("Vanilla_Cfr_1000_StrategySum");
    cfr[3]->readFile("Vanilla_Cfr_10000_RegretSum");
    cfr[3]->readFile("Vanilla_Cfr_10000_StrategySum");

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            battle_array[i][j] = 0;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            battle_array[i][j] = battle(game, root, cfr[i], cfr[j], 1000);
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << battle_array[i][j] << " ";
        }

        std::cout << "\n";
    }*/

    calculateFiveCardBucket(10);
    return 0;
}

