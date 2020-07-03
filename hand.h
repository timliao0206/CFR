#pragma once
#ifndef HAND_H
#define HAND_H

#include <inttypes.h>
#include <vector>

extern "C" {
#include "acpc_code/game.h"
	using namespace project_acpc_server;
};


class Hand {
public:

	int8_t board_cards[MAX_BOARD_CARDS];
	int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS];

	int precomputed_bucket[MAX_PLAYERS][MAX_ROUNDS];

	int8_t show_down_value[2];
};

Hand generateHand(const Game* game);

int dealCard(std::vector<int>& desk);

#endif // !HAND_H
