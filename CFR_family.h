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

class CFR {
public:

	virtual void doIteration(const BettingNode* root) = 0;
	virtual void doIteration(const BettingNode* root, const int times) = 0;
	virtual double getExploitability(const BettingNode* root) const = 0;

	virtual int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	virtual int getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	virtual void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);
	virtual void updateStrategySum(const BettingNode* node, const Hand hand, const int position, std::vector<double> strategy);

	virtual void printRegretSum(std::string fileName) const;
	virtual void printStrategySum(std::string fileName) const;

	virtual void readFile(std::string fileName);
protected:

	virtual std::string methodName() const = 0;

	Storage* stored[MAX_ROUNDS];
	const Game* game;
	const CardAbstraction* card_abs;
};

class VanillaCfr : public CFR{
public:

	VanillaCfr(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	
	~VanillaCfr();

	virtual void doIteration(const BettingNode* root);
	virtual void doIteration(const BettingNode* root, const int times);

	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);

	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);
	virtual double getExploitability(const BettingNode* root) const;

	static int go_through;

protected:
	virtual std::string methodName() const { return "VANILLA_CFR"; }
	
private:

	double computeExploitability(const BettingNode* node, const Hand hand, const int position) const;
};

class VanillaCfr_RPB : public CFR{
public:

	VanillaCfr_RPB(const Game* game, const RealProbBucketing_train* RPB, size_t num_entries_per_bucket[MAX_ROUNDS]);

	virtual void doIteration(const BettingNode* root, const int times);
	virtual void doIteration(const BettingNode* root);

	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);

	virtual double getExploitability(const BettingNode* root) const;

	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	int getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);
	void updateStrategySum(const BettingNode* node,  const Hand hand, const int position,std::vector<double> strategy);

protected:
	virtual std::string methodName() const{ return "VANILLA_CFR_RPB"; }

private:
	const RealProbBucketing_train* RPB;

	double computeExploitability(const BettingNode* node, const Hand hand, const int position) const;
};

class ES : public CFR{
public:

	ES(const Game* game , size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	~ES();

	virtual void doIteration(const BettingNode* root);
	virtual void doIteration(const BettingNode* root, const int time);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand);

	int sampleAction(const BettingNode* node, const Hand hand, const int position);

	virtual double getExploitability(const BettingNode* root) const;

	static int go_through;

protected:
	virtual std::string methodName() const{ return "ES"; }

private:
	double computeExploitability(const BettingNode* node,const Hand hand, const int position) const;

};

#endif // !CFR_FAMILY_H
