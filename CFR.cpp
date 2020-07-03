#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "betting_node.h"
#include <fstream>
#include <cassert>
#include <stack>
#include "CFR_family.h"
#include "evalHandTable.h"

int main()
{
    /*FILE* file;
    file = fopen("games/holdem.limit.2p.reverse_blinds.GAME", "r");

    const Game* game = readGame(file);

    State state;
    initState(game, 0 , &state);

    const NullActionAbstraction* abs = new NullActionAbstraction();

    size_t num_entries_per_bucket[MAX_ROUNDS] = { 0,0,0,0 };

    const BettingNode* root = initBettingTree(state, game, abs, num_entries_per_bucket);

    Hand hand = generateHand(game);

    int num_bucket[4] = { 10,10,10,10 };
    const CardAbstraction* card_abs = new EHS_Bucketing(num_bucket);

    ES::go_through = 0;

    ES cfr(game, num_entries_per_bucket, card_abs);

    cfr.doIteration(root);

    std::cout << ES::go_through << std::endl;*/
    std::cout << "{";
    for (int i = 0; i < 51; i++) {
        for (int j = i + 1; j < 52; j++) {
            std::cout << i << ",";
        }
        std::cout << "\n";
    }

    return 0;
}

