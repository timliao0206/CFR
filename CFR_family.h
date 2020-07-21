#pragma once
#ifndef CFR_FAMILY_H
#define CFR_FAMILY_H

#include "hand.h"
#include "betting_node.h"
#include "storage.h"
#include "card_abstraction.h"

#include <vector>
#include <string>

extern "C" {
#include "acpc_code/game.h"
}

class VanillaCfr{
public:

	VanillaCfr(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	
	~VanillaCfr();

	void doIteration(const BettingNode* root);
	void doIteration(const BettingNode* root, const int times);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);
	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);
	double getExploitabilitybyMonte(const BettingNode* root) const;

	void printRegretSum(std::string fileName);

	static int go_through;

private:

	double computeExploitability(const BettingNode* node, const Hand hand, const int position) const;

	Storage* stored[MAX_ROUNDS];
	const CardAbstraction* card_abs;
	const Game* game;
};

class VanillaCfr_RPB {
public:

	VanillaCfr_RPB(const Game* game, const RealProbBucketing_train* RPB, size_t num_entries_per_bucket[MAX_ROUNDS]);
	void doIteration(const BettingNode* root, const int times);
	double getExploitability(const BettingNode* root) const;
	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	int getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);
	void updateStrategySum(const BettingNode* node,  const Hand hand, const int position,std::vector<double> strategy);

	void printRegretSum(std::string fileName);
	void printStrategySum(std::string fileName);
	void readFile(std::string fileName);

private:

	Storage* stored[MAX_ROUNDS];
	const CardAbstraction* card_abs;
	const Game* game;
	const RealProbBucketing_train* RPB;
	double computeExploitability(const BettingNode* node, const Hand hand, const int position) const;
};

class ES{
public:

	ES(const Game* game , size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	~ES();

	void doIteration(const BettingNode* root);
	void doIteration(const BettingNode* root, const int time);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand);
	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);

	int sampleAction(const BettingNode* node, const Hand hand, const int position);

	double getExploitabilitybyMonte(const BettingNode* root) const;

	static int go_through;

	void printRegretSum(std::string fileName);

private:
	double computeExploitability(const BettingNode* node,const Hand hand, const int position) const;


	Storage* stored[MAX_ROUNDS];
	const CardAbstraction* card_abs;
	const Game* game;
};

#endif // !CFR_FAMILY_H
