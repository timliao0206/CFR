#include "hand.h"
#include "evalHandTable.h"
#include <vector>
#include <assert.h>
#include <iostream>

extern "C" {
#include "acpc_code/rng.h"
using namespace project_acpc_server;
}

Hand generateHand(const Game* game) {

	std::vector<int> desk;
	for (int i = 0; i < (game->numRanks) * (game->numSuits); i++) {
		desk.push_back(i);
	}

	std::vector<CardSet> sets(game->numPlayers);

	Hand hand;

	for (int i = 0; i < game->numPlayers; i++) {
		sets[i] = eval::emptyCardset();
		for (int j = 0; j < game->numHoleCards; j++) {
			hand.hole_cards[i][j] = dealCard(desk);
			eval::addCardToCardSet(&sets[i], hand.hole_cards[i][j] / 13, hand.hole_cards[i][j] % 13);
		}
	}

	int numBoardCard = 0;
	for (int i = 0; i < game->numRounds; i++) {
		numBoardCard += game->numBoardCards[i];
	}

	for (int i = 0; i < numBoardCard; i++) {
		hand.board_cards[i] = dealCard(desk);

		for (int j = 0; j < game->numPlayers; j++) {
			eval::addCardToCardSet(&sets[j], hand.board_cards[i] / 13, hand.board_cards[i] % 13);
		}
	}

	//2P only
	int rank_p1 = eval::rankCardset(sets[0]);
	int rank_p2 = eval::rankCardset(sets[1]);

	if (rank_p1 > rank_p2) {
		hand.show_down_value[0] = 1;
		hand.show_down_value[1] = -1;
	}
	else if (rank_p1 < rank_p2) {
		hand.show_down_value[0] = -1;
		hand.show_down_value[1] = 1;
	}
	else {
		hand.show_down_value[0] = 0;
		hand.show_down_value[1] = 0;
	}

	return hand;
}

int dealCard(std::vector<int>& desk) {

	int ran = rand();
	ran *= desk.size();
	ran /= (RAND_MAX+1);

	assert(ran < desk.size());

	int ans = desk[ran];
	desk.erase(desk.begin() + ran);

	return ans;
}

Hand generateEmptyHand() {
	Hand hand;
	for (int i = 0; i < MAX_BOARD_CARDS; i++) {
		hand.board_cards[i] = -1;
	}

	for (int i = 0; i < MAX_PLAYERS; i++) {
		for (int j = 0; j < MAX_HOLE_CARDS; j++) {
			hand.hole_cards[i][j] = -1;
		}

		for (int j = 0; j < MAX_ROUNDS; j++) {
			hand.precomputed_bucket[i][j] = -1;
		}
	}

	hand.show_down_value[0] = 0;
	hand.show_down_value[1] = 0;

	return hand;
}
