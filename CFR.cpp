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
    initState(game, 0, &state);

    const NullActionAbstraction* abs = new NullActionAbstraction();

    std::array<int, MAX_ROUNDS> raise_time = { 3,3,3,3 };

    const PropotionalActionAbstraction* pabs = new PropotionalActionAbstraction(raise_time, 3);


    Action actions[11];

    int num_action = pabs->getActions(game, state, actions);


    size_t num_entries_per_bucket[MAX_ROUNDS] = { 0,0,0,0 };

    BettingNode::size = 0;

    const BettingNode* root = initBettingTree(state, game, abs, num_entries_per_bucket);

    int num_bucket[MAX_ROUNDS] = { 10,10,10,10 };

    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);

    //initializing unevenEHS 

    vector<double> bucket_density[MAX_ROUNDS];
    double density_array_Pre[] = {0.05,0.05735,0.06578,0.07545,0.08654,0.09926,0.11385,0.13059,0.14979,0.17139};
    double density_array_else[] = { 0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1 };
    bucket_density[0].insert(bucket_density[0].end(), &density_array_Pre[0], &density_array_Pre[10]);
    for (int i = 1; i <= 3; i++)
        bucket_density[i].insert(bucket_density[i].end(), &density_array_else[0], &density_array_else[10]);


    
    const CardAbstraction* uneven_card_abs = new unevenEHS(num_bucket,bucket_density);

    /*ES* cfr = new ES(game, num_entries_per_bucket, card_abs);

    cfr->readFile("RunData/ES_10000000_RegretSum");
    cfr->readFile("RunData/ES_10000000_StrategySum");

    const BettingNode* cur(root);
    std::queue<int> action_sequence;
    for (int i = 0; i < 3; i++) {
        cur = cur->doAction(2);
        action_sequence.push(2);

        cur = cur->doAction(1);
        action_sequence.push(1);
    }

    int8_t arr[7] = { 12,25,37,23,24,52,52 };
    int8_t hole[10][3];
    hole[1][0] = 38;
    hole[1][1] = 51;
    hole[1][2] = 52;
    hole[0][0] = 22;
    hole[0][1] = 21;
    hole[0][2] = 52;
    
    Hand hand = generateGivenHand(arr, hole);
    double val = cfr->expectedValue(root, action_sequence,cur, hand, 0);

    std::cout << val;*/
    /*
    calculateFiveCardBucket(10);*/



    /*


    ES cfr(game,num_entries_per_bucket,uneven_card_abs);

    const long long iteration = 1e6;

    int start_time = time(0);

    std::string fileHead = "Research/experiment02/unevenES_";
    std::string fileTail_r = "_RegretSum";
    std::string fileTail_s = "_StrategySum";

    for (long long i = 0; i < iteration; i++) {
        cfr.doIteration(root,1);

        if (i == iteration/1000 || i == iteration/100 || i == iteration/10) {
            cfr.printRegretSum(fileHead + std::to_string(i) + fileTail_r);
            cfr.printStrategySum(fileHead + std::to_string(i) + fileTail_s);
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

    cfr.printRegretSum(fileHead + std::to_string(iteration) + fileTail_r);
    cfr.printStrategySum(fileHead + std::to_string(iteration) + fileTail_s);

    */

    /*for (int i = 0; i < 4; i++) {
        cfr[i] = new VanillaCfr_RPB(game, num_entries_per_bucket, RPB);
    }*/
    
    std::fstream battle_file;
    battle_file.open("Research/experiment02/battleFile01",std::ios::out);

    /*
    ES* EHS[3];
    ES* unevenEHS[3];


    RandomPlayer* random_player = new RandomPlayer(game,card_abs);

    for (int i = 0; i < 3; i++) {
        EHS[i] = new ES(game, num_entries_per_bucket, card_abs);
        unevenEHS[i] = new ES(game, num_entries_per_bucket, uneven_card_abs);
    }

    int iter = 10000;

    for (int i = 0; i < 3; i++) {
        EHS[i]->readFile("Research/experiment02/ES_"+std::to_string(iter) + "_RegretSum");
        unevenEHS[i]->readFile("Research/experiment02/unevenES_" + std::to_string(iter) + "_RegretSum");
        iter *= 10;
    }

    std::cout << "Load file complete";
    
    battle_file << "EHS self:\n";
    battle_file << "4 vs 5 : " << battle(game, root, EHS[0], EHS[1], 1000) << "\n";
    battle_file << "4 vs 6 : " << battle(game, root, EHS[0], EHS[2], 1000) << "\n";
    battle_file << "5 vs 6 : " << battle(game, root, EHS[1], EHS[2], 1000) << "\n";

    battle_file << "unevenEHS self:\n";
    battle_file << "4 vs 5 : " << battle(game, root, unevenEHS[0], unevenEHS[1], 1000) << "\n";
    battle_file << "4 vs 6 : " << battle(game, root, unevenEHS[0], unevenEHS[2], 1000) << "\n";
    battle_file << "5 vs 6 : " << battle(game, root, unevenEHS[1], unevenEHS[2], 1000) << "\n";

    battle_file << "EHS vs unevenEHS : \n";
    battle_file << "4 : " << battle(game, root, EHS[0], unevenEHS[0], 1000) << "\n";
    battle_file << "5 : " << battle(game, root, EHS[1], unevenEHS[1], 1000) << "\n";
    battle_file << "6 : " << battle(game, root, EHS[2], unevenEHS[2], 1000) << "\n";
    */
    //battle_file << "6 vs 6 : " << battle(game, root, EHS[2], EHS[2], 1000) << "\n";
    //battle_file << "6 vs 6 : " << battle(game, root, unevenEHS[2], unevenEHS[2], 1000) << "\n";
    /*ES* es[2];
    es[0] = new ES(game, num_entries_per_bucket, card_abs);
    es[1] = new ES(game, num_entries_per_bucket,card_abs);
    es[0]->readFile("RunData/ES_9000000_RegretSum");
    es[1]->readFile("RunData/ES_10000000_RegretSum");

    battle_file << battle(game, root, es[0], es[1], 1000);*/
    ES* cfr[10];
    //RandomPlayer* random_player = new RandomPlayer(game, card_abs);
    for (int i = 0; i < 10; i++) {
        cfr[i] = new ES(game, num_entries_per_bucket, card_abs);
        //os[i] = new OS(game, num_entries_per_bucket, card_abs);
    }
    for (int i = 1; i <= 10; i++) {
        cfr[i - 1]->readFile("RunData/ES_" + std::to_string(i) + "000000_RegretSum");
    }
    for (int i = 0; i < 9; i++) {
        battle_file << "ES with" << i+1 <<"X10^6" << " iteration:\n";
        for (int j = i+1; j <= 9; j++) {
            battle_file << "ES with " << j + 1 << "X10^6 iteration " << battle(game, root, cfr[i], cfr[j], 1000) << "\n";
        }
        battle_file << "\n";
    }
    /*
    for (int i = 0; i < 4; i++) {
        std::cout << "Vanilla Cfr with 10^" << i + 1 << "iteration vs Random : " << battle(game, root, van[i], random_player, 10000) << "\n";
    }

    for (int i = 0; i < 4; i++) {
        std::cout << "ES with 10^" << i + 3 << "iteration vs Random : " << battle(game, root, cfr[i], random_player, 10000) << "\n";
    }*/
}
