#pragma once
#ifndef CFR_FAMILY_H
#define CFR_FAMILY_H

#include "hand.h"
#include "betting_node.h"
#include "storage.h"
#include "card_abstraction.h"

#include <vector>

extern "C" {
#include "acpc_code/game.h"
}
class VanillaCfr{
public:

	VanillaCfr(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	~VanillaCfr();

	void doIteration(const BettingNode* root);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);
	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy);
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);

	static int go_through;

private:

	Storage* stored[MAX_ROUNDS];
	const CardAbstraction* card_abs;
	const Game* game;
};

class ES{
public:

	ES(const Game* game , size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	~ES();

	void doIteration(const BettingNode* root);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand);
	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy);
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);

	int sampleAction(const BettingNode* node, const Hand hand, const int position);

	static int go_through;

private:

	Storage* stored[MAX_ROUNDS];
	const CardAbstraction* card_abs;
	const Game* game;
};

#endif // !CFR_FAMILY_H
