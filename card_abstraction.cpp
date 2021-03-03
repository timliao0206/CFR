#include "card_abstraction.h"
#include "evalHandTable.h"
#include <vector>
#include <algorithm>
#include <ppl.h>
#include "FiveCardBucket.h"

CardAbstraction::CardAbstraction() {

}

CardAbstraction::~CardAbstraction() {

}

//get a vector which containing all possible hand , and it's element indicates the hand strength between
//hole card and the representation of it's index.1 = hole card win, 0 = draw , -1 = opponent win.
void CardAbstraction::getDefeatedHand(const Game* game, const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_HOLE_CARDS], std::vector<int>& defeated)const {
	
	defeated.resize(1326, 0);

	CardSet board = eval::emptyCardset();
	
	for (int i = 0; i < 5; i++) {
		eval::addCardToCardSet(&board, board_cards[i] / 13, board_cards[i] % 13);
	}

	CardSet self(board);

	eval::addCardToCardSet(&self, hole_cards[0] / 13, hole_cards[0] % 13);
	eval::addCardToCardSet(&self, hole_cards[1] / 13, hole_cards[1] % 13);

	int self_hand_point = eval::rankCardset(self);

	for (int i = 0; i < 1326; i++) {
		int card1 = index_to_card_one[i];
		int card2 = index_to_card_two[i];
		
		if (card1 == board_cards[0] || card1 == board_cards[1] || card1 == board_cards[2] || card1 == board_cards[3] || card1 == board_cards[4] || card1 == hole_cards[0] || card1 == hole_cards[1] ||
			card2 == board_cards[0] || card2 == board_cards[1] || card2 == board_cards[2] || card2 == board_cards[3] || card2 == board_cards[4] || card2 == hole_cards[0] || card2 == hole_cards[1]) {
			defeated[i] = 0;
			continue;
		}

		CardSet opponent(board);
		eval::addCardToCardSet(&opponent, card1 / 13, card1 % 13);
		eval::addCardToCardSet(&opponent, card2 / 13, card2 % 13);

		int opponent_hand_point = eval::rankCardset(opponent);

		if (self_hand_point > opponent_hand_point)
			defeated[i] = 1;
		else if (opponent_hand_point > self_hand_point)
			defeated[i] = -1;
		else
			defeated[i] = 0;
	}
}

const int CardAbstraction::index_to_card_two[1326] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
38,39,40,41,42,43,44,45,46,47,48,49,50,51,
39,40,41,42,43,44,45,46,47,48,49,50,51,
40,41,42,43,44,45,46,47,48,49,50,51,
41,42,43,44,45,46,47,48,49,50,51,
42,43,44,45,46,47,48,49,50,51,
43,44,45,46,47,48,49,50,51,
44,45,46,47,48,49,50,51,
45,46,47,48,49,50,51,
46,47,48,49,50,51,
47,48,49,50,51,
48,49,50,51,
49,50,51,
50,51,
51};
const int CardAbstraction::index_to_card_one[1326] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,
25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,
28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,
30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,
34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,
35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,
36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,
37,37,37,37,37,37,37,37,37,37,37,37,37,37,
38,38,38,38,38,38,38,38,38,38,38,38,38,
39,39,39,39,39,39,39,39,39,39,39,39,
40,40,40,40,40,40,40,40,40,40,40,
41,41,41,41,41,41,41,41,41,41,
42,42,42,42,42,42,42,42,42,
43,43,43,43,43,43,43,43,
44,44,44,44,44,44,44,
45,45,45,45,45,45,
46,46,46,46,46,
47,47,47,47,
48,48,48,
49,49,
50 };

EHS_Bucketing::EHS_Bucketing(const int num_buckets[MAX_ROUNDS]){
	for (int i = 0; i < MAX_ROUNDS; i++) {
		m_num_buckets[i] = num_buckets[i];
	}
}

EHS_Bucketing::~EHS_Bucketing(){}

int EHS_Bucketing::numBuckets(const Game* game, const BettingNode* node) const {
	return m_num_buckets[node->getRound()];
}

int EHS_Bucketing::numBuckets(const Game* game, const State& state) const {
	return m_num_buckets[state.round];
}

int EHS_Bucketing::getBucket(const Game* game, const BettingNode* node,
	const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const {
	
	//pre-flop , which has been calculated beforehand.
	if (node->getRound() == 0) {
		return getBucket_preflop(board_cards, hole_cards, position);
	}

	//flop
	if (node->getRound() == 1) {
		return getBucket_flop(board_cards, hole_cards, position);
	}

	//turn
	if (node->getRound() == 2) {
		return getBucket_turn(board_cards, hole_cards, position);
	}

	//river
	if (node->getRound() == 3) {
		return getBucket_river(board_cards, hole_cards, position);
	}

	assert(false);
}

int EHS_Bucketing::getBucket_preflop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], 
	const int position) const {

	return  ((eval::rankTwoCards(hole_cards[position][0],hole_cards[position][1]) - 1) * m_num_buckets[0]) / 169;
}

int EHS_Bucketing::getBucket_flop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS],
			const int position) const {

	return getFlopBucketByHandStrength(board_cards,hole_cards,position,(uint8_t)m_num_buckets[1]);
}

int EHS_Bucketing::getBucket_turn(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS],
			const int position) const {

	/*std::vector< std::pair<int, double> > all_hole_cards;
	all_hole_cards.resize(1326);
	for (int i = 0; i < 1326; i++) {
		all_hole_cards[i] = std::make_pair(i, 0);
	}

	CardSet board = eval::emptyCardset();
	for (int i = 0; i < 4; i++) {
		eval::addCardToCardSet(&board, board_cards[i] / 13, board_cards[i] % 13);
	}

	for (int i = 0; i < 52; i++) {
		if (i == board_cards[0] || i == board_cards[1] ||
			i == board_cards[2] || i == board_cards[3] || i == board_cards[4]) continue;

		CardSet possible_river_board = board;
		eval::addCardToCardSet(&possible_river_board, i / 13, i % 13);

		std::vector< std::pair<int, int> > hole_cards_rank;
		hole_cards_rank.resize(1326);
		for (int j = 0; j < 1326; j++) {
			hole_cards_rank[i] = std::make_pair(i, 0);
		}

		for (int j = 0; j < 1326; j++) {
			int hole_card_one = index_to_card_one[j];
			int hole_card_two = index_to_card_two[j];

			bool flag = false;
			for (int t = 0; t < 5; t++) {
				flag |= hole_card_one == board_cards[t];
				flag |= hole_card_two == board_cards[t];
			}

			if (flag) {
				hole_cards_rank[j].second = -1;
				continue;
			}

			CardSet hand = possible_river_board;
			eval::addCardToCardSet(&hand, hole_card_one / 13, hole_card_one % 13);
			eval::addCardToCardSet(&hand, hole_card_two / 13, hole_card_two % 13);

			hole_cards_rank[j].second = eval::rankCardset(hand);
		}

		std::sort(hole_cards_rank.begin(), hole_cards_rank.end(), [](std::pair<int, int> a, std::pair<int, int> b) {return a.second > b.second; });

		int count = 1326;
		for (int t = hole_cards_rank.size() - 1; t >= 0; t--) {
			if (hole_cards_rank[t].second != -1) break;
			count--;
		}

		for (int t = 0; t < 1326; t++) {
			//TO-DO
			//same rank to be done.
		}
	}*/
	
	return getFlopBucketByHandStrength(board_cards, hole_cards, position, (uint8_t)m_num_buckets[1]);
}

int EHS_Bucketing::getBucket_river(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS],
	const int position) const {

	CardSet board = eval::emptyCardset();
	for (int i = 0; i < 5; i++) {
		eval::addCardToCardSet(&board, board_cards[i] / 13, board_cards[i] % 13);
	}

	CardSet self = board;
	for (int i = 0; i < 2; i++) {
		eval::addCardToCardSet(&self, hole_cards[position][i] / 13, hole_cards[position][i] % 13);
	}

	int self_rank = eval::rankCardset(self);
	int lose_cardset = 0;


	for (int i = 0; i < 51; i++) {
		if (i == board_cards[0] || i == board_cards[1] ||
			i == board_cards[2] || i == board_cards[3] || i == board_cards[4]) continue;

		for (int j = i + 1; j < 52; j++) {
			if (j == board_cards[0] || j == board_cards[1] ||
				j == board_cards[2] || j == board_cards[3] || j == board_cards[4]) continue;

			CardSet tmp = board;
			eval::addCardToCardSet(&tmp, i / 13, i % 13);
			eval::addCardToCardSet(&tmp, j / 13, j % 13);

			if (self_rank < eval::rankCardset(tmp)) lose_cardset++;
		}
	}

	return lose_cardset * m_num_buckets[3] / 1326;
}

void EHS_Bucketing::getBucketForAllHand(const Game* game, const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int> buckets[4])const {

	for (int i = 0; i < 4; i++) {
		getBucketForAllHand(game, i, board_cards, buckets[i]);
	}

	return;
}

void EHS_Bucketing::getBucketForAllHand(const Game* game, const BettingNode* node,
	const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const {

	buckets.resize(1326, -1);

	switch (node->getRound()) {
	case 0:
		getBucketAll_preflop(board_cards, buckets);
		break;
	case 1:
		getBucketAll_flop(board_cards, buckets);
		break;
	case 2:
		getBucketAll_turn(board_cards, buckets);
		break;
	case 3:
		getBucketAll_river(board_cards, buckets);
		break;

	default:
		assert(0);
	}

	return;
}

void EHS_Bucketing::getBucketForAllHand(const Game* game, const int round,
	const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const {

	buckets.resize(1326, -1);

	switch (round) {
	case 0:
		getBucketAll_preflop(board_cards, buckets);
		break;
	case 1:
		getBucketAll_flop(board_cards, buckets);
		break;
	case 2:
		getBucketAll_turn(board_cards, buckets);
		break;
	case 3:
		getBucketAll_river(board_cards, buckets);
		break;

	default:
		assert(0);
	}

	return;
}

void EHS_Bucketing::getBucketAll_preflop(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const{

	/*concurrency::parallel_for(0, 1326, [&](int i) {
		int c1 = index_to_card_one[i];
		int c2 = index_to_card_two[i];

		buckets[i] = 
			(c1 == board_cards[0] || c1 == board_cards[1] || c1 == board_cards[2] ||
			c2 == board_cards[0] || c2 == board_cards[1] || c2 == board_cards[2]) ? 
			((eval::rankTwoCards(c1, c2) - 1) * m_num_buckets[0] / 169) : 0;
		});
	*/
	for (int i = 0; i < 1326; i++) {
		int c1 = index_to_card_one[i];
		int c2 = index_to_card_two[i];

		if (c1 == board_cards[0] || c1 == board_cards[1] || c1 == board_cards[2] ||
			c2 == board_cards[0] || c2 == board_cards[1] || c2 == board_cards[2]) {
			buckets[i] = 0;
			continue;
		}

		buckets[i] = (eval::rankTwoCards(c1, c2) - 1) * m_num_buckets[0] / 169;
	}
}

void EHS_Bucketing::getBucketAll_flop(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const {
	getAllPossibleFlopBucketByHandStrength(board_cards, buckets,10);
}

void EHS_Bucketing::getBucketAll_turn(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const {
	getAllPossibleFlopBucketByHandStrength(board_cards, buckets, 10);
}

bool pairComp(std::pair<int, int> p1, std::pair<int, int> p2) {
	return p1.second > p2.second;
}

void EHS_Bucketing::getBucketAll_river(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const {

	std::vector< std::pair<int,int> > hand_point;

	CardSet board_cardset = eval::emptyCardset();

	for (int i = 0; i < 5; i++) {
		eval::addCardToCardSet(&board_cardset, board_cards[i] / 13, board_cards[i] % 13);
	}

	for (int i = 0; i < 1326; i++) {
		int c1 = index_to_card_one[i];
		int c2 = index_to_card_two[i];

		if (c1 == board_cards[0] || c1 == board_cards[1] || c1 == board_cards[2] ||
			c2 == board_cards[0] || c2 == board_cards[1] || c2 == board_cards[2]){

			hand_point.push_back(std::make_pair(i, 0));
			continue;
		}

		CardSet hand(board_cardset);
		eval::addCardToCardSet(&hand, c1 / 13, c1 % 13);
		eval::addCardToCardSet(&hand, c2 / 13, c2 % 13);

		hand_point.push_back(std::make_pair(i, eval::rankCardset(hand)));
	}

	std::sort(hand_point.begin(), hand_point.end(), pairComp);

	int rank = 0;
	int prev = -1;

	for (int i = 0; i < hand_point.size(); i++) {
		buckets[hand_point[i].first] = rank * m_num_buckets[3] / 1326;

		if (prev != hand_point[i].second) {
			prev = hand_point[i].second;
			rank = i+1;
		}
	}

	return;
}

bool EHS_Bucketing::canPrecomputeBuckets() const {
	return true;
}

void EHS_Bucketing::precomputeBuckets(const Game* game, Hand& hand) const {
	for (int i = 0; i < game->numPlayers; i++) {
		hand.precomputed_bucket[i][0] = getBucket_preflop(hand.board_cards, hand.hole_cards, i);
		hand.precomputed_bucket[i][1] = getBucket_flop(hand.board_cards, hand.hole_cards, i);
		hand.precomputed_bucket[i][2] = hand.precomputed_bucket[i][1];//tmp
		hand.precomputed_bucket[i][3] = getBucket_river(hand.board_cards, hand.hole_cards, i);
	}
}

void EHS_Bucketing::precomputeBuckets(const Game* game, Hand& hand, const int position) const{
	hand.precomputed_bucket[position][0] = getBucket_preflop(hand.board_cards, hand.hole_cards, position);
	hand.precomputed_bucket[position][1] = getBucket_flop(hand.board_cards, hand.hole_cards, position);
	hand.precomputed_bucket[position][2] = hand.precomputed_bucket[position][1];//tmp
	hand.precomputed_bucket[position][3] = getBucket_river(hand.board_cards, hand.hole_cards, position);
}

RealProbBucketing_train::RealProbBucketing_train(){}

RealProbBucketing_train::~RealProbBucketing_train(){}

void RealProbBucketing_train::precomputeBuckets(Hand& hand, int prob[MAX_PLAYERS]) const{

	for (int round = 0; round < MAX_ROUNDS; round++) {
		for (int player = 0; player < MAX_PLAYERS; player++) {
			hand.precomputed_bucket[player][round] = prob[player];
		}
	}

	double p1_win = 0;
	double p2_win = 0;

	if ((prob[0] != 0 || prob[1] != 0) && (prob[0] != 10 || prob[1] != 10)) {
		p1_win = (double)(prob[0]) * (10 - prob[1]) / ( (prob[0]) * (10 - prob[1]) + (prob[1]) * (10 - prob[0]) );
		p2_win = (double)(prob[1]) * (10 - prob[0]) / ((prob[0]) * (10 - prob[1]) + (prob[1]) * (10 - prob[0]));
	}
	else {
		hand.show_down_value[0] = 0;
		hand.show_down_value[1] = 0;
		return;
	}

	double ran = (double)rand() / (double)(RAND_MAX + 1.0);

	if (ran >= p1_win) {
		hand.show_down_value[0] = -1;
		hand.show_down_value[1] = 1;
	}
	else {
		hand.show_down_value[0] = 1;
		hand.show_down_value[1] = -1;
	}

	return;
}

unevenEHS::unevenEHS(const int num_buckets[MAX_ROUNDS],vector<double> bucket_density[MAX_ROUNDS]) : EHS_Bucketing(num_buckets)
{
	for (int i = 0; i < MAX_ROUNDS; i++) {
		m_num_buckets[i] = num_buckets[i];
		m_bucket_density[i].resize(10);

		double check_one = 0;
		for (int j = 0; j < m_num_buckets[i]; j++) {
			m_bucket_density[i][j] = bucket_density[i][j];
			check_one += bucket_density[i][j];
		}

		assert(abs(check_one - 1) < 10e-5);
	}
}

int unevenEHS::getBucket_preflop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const {
	int rank = eval::rankTwoCards(hole_cards[position][0], hole_cards[position][1]) - 1;

	return preflopRankToBucket(rank);
}

void unevenEHS::getBucketAll_preflop(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const {

	for (int i = 0; i < 1326; i++) {
		int c1 = index_to_card_one[i];
		int c2 = index_to_card_two[i];

		if (c1 == board_cards[0] || c1 == board_cards[1] || c1 == board_cards[2] ||
			c2 == board_cards[0] || c2 == board_cards[1] || c2 == board_cards[2]) {
			buckets[i] = 0;
			continue;
		}

		int rank = eval::rankTwoCards(c1, c2) - 1;
		buckets[i] = preflopRankToBucket(rank);
	}
}

int unevenEHS::preflopRankToBucket(int rank)const {
	double PR = (double)rank / 169.0;
	int bucket = 0;
	double accu = 0;
	while (accu <= PR) {
		if (bucket == 10)
			break;
		accu += m_bucket_density[0][bucket];
		bucket++;
	}
	bucket--;

	return bucket;
}

testEHS::testEHS(const int num_buckets[MAX_ROUNDS]):EHS_Bucketing(num_buckets) {}

int testEHS::getBucket_preflop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const {
	return (eval::rankTwoCards(hole_cards[position][0], hole_cards[position][1]) - 1) % m_num_buckets[0];
}

void testEHS::getBucketAll_preflop(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const {
	for (int i = 0; i < 1326; i++) {
		int c1 = index_to_card_one[i];
		int c2 = index_to_card_two[i];

		if (c1 == board_cards[0] || c1 == board_cards[1] || c1 == board_cards[2] ||
			c2 == board_cards[0] || c2 == board_cards[1] || c2 == board_cards[2]) {
			buckets[i] = 0;
			continue;
		}

		buckets[i] = (eval::rankTwoCards(c1, c2) - 1) % m_num_buckets[0];
	}
}