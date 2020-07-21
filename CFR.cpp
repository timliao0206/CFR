#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "betting_node.h"
#include <fstream>
#include <cassert>
#include <stack>
#include <time.h>
#include <chrono>
#include "CFR_family.h"
#include "evalHandTable.h"
#include <ctime>

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

    /*int num_bucket[4] = { 10,10,10,10 };
    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);

    ES::go_through = 0;

    VanillaCfr cfr(game, num_entries_per_bucket, card_abs);

    */

    const RealProbBucketing_train* RPB = new RealProbBucketing_train();

    VanillaCfr_RPB cfr(game, RPB, num_entries_per_bucket);

    cfr.readFile("RPB_RegretSum.txt");
    cfr.readFile("RPB_StrategySum.txt");

    std::fstream writeFile;
    writeFile.open("Exploitability_Vanilla_RPB.txt", std::ios::out);

    if (!writeFile) {
        std::cout << "Can't open file." << std::endl;
        return 0;
    }

    std::cout << "Successfully open the file." << std::endl;

    auto curr_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    writeFile << "Current Time : " << std::ctime(&curr_time) << "\n";
    writeFile << "RPB + vanilla cfr , get exploitability by average strategy";

    const int printFreq = 1;

    for (int i = 0; i < 100; i++) {
        //int start = time(0);

        cfr.doIteration(root, printFreq);

        //int end = time(0);

        //writeFile << (i + 1) * printFreq << "   " << cfr.getExploitability(root) << "\n";

        std::cout << (i + 1) * printFreq << "th iteration complete.\n";

        std::string fileName = "RPB_RegretSum.txt";
        std::string file_strategy = "RPB_StrategySum.txt";

        cfr.printRegretSum(fileName);
        cfr.printStrategySum(file_strategy);

        break;
    }
    return 0;
}

