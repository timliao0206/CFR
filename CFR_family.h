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
	virtual int sampleAction(const BettingNode* node, const Hand hand, const int position) const;

	virtual void printRegretSum(std::string fileName) const;
	virtual void printStrategySum(std::string fileName) const;

	virtual void readFile(std::string fileName);

	virtual const CardAbstraction* getCardAbstraction() const{ return card_abs; }
	virtual const Game* getGame() const{ return game; }
protected:

	virtual std::string methodName() const = 0;

	Storage* stored[MAX_ROUNDS];
	const Game* game;
	const CardAbstraction* card_abs;
};

class RandomPlayer :public CFR {
public:

	RandomPlayer(const Game* game, const CardAbstraction* card_abs);

	virtual void doIteration(const BettingNode* root) { assert(0); };
	virtual void doIteration(const BettingNode* root, const int times) { assert(0); };
	virtual double getExploitability(const BettingNode* root) const { assert(0); return 0; };

	virtual int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	virtual int getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	virtual void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret) { return; };
	virtual void updateStrategySum(const BettingNode* node, const Hand hand, const int position, std::vector<double> strategy) { return; };
	virtual int sampleAction(const BettingNode* node, const Hand hand, const int position) const;

	virtual void printRegretSum(std::string fileName) const { assert(0); };
	virtual void printStrategySum(std::string fileName) const { assert(0); };

	virtual void readFile(std::string fileName) { return; };

protected:
	virtual std::string methodName() const { return "RANDOM_PLAYER"; };
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

	VanillaCfr_RPB(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS],const RealProbBucketing_train* RPB);

	virtual void doIteration(const BettingNode* root, const int times);
	virtual void doIteration(const BettingNode* root);

	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);

	virtual double getExploitability(const BettingNode* root) const;

	int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	int getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);
	void updateStrategySum(const BettingNode* node,  const Hand hand, const int position,std::vector<double> strategy);

	virtual const RealProbBucketing_train* getCardAbstraction() const{ return RPB; }
protected:
	virtual std::string methodName() const{ return "VANILLA_CFR_RPB"; }
	const RealProbBucketing_train* RPB;
private:
	

	double computeExploitability(const BettingNode* node, const Hand hand, const int position) const;
};

class ES : public CFR {
public:

	ES(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	~ES();

	virtual void doIteration(const BettingNode* root);
	virtual void doIteration(const BettingNode* root, const int time);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand);

	virtual double getExploitability(const BettingNode* root) const;

	//virtual int sampleAction(const BettingNode* node, const Hand hand, const int position)const;

	static int go_through;

protected:
	virtual std::string methodName() const{ return "ES"; }

private:
	double computeExploitability(const BettingNode* node,const Hand hand, const int position) const;

};

class OS : public CFR {
public:

	OS(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs);
	virtual void doIteration(const BettingNode* root);
	virtual void doIteration(const BettingNode* root, const int time);
	double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob);

	virtual double getExploitability(const BettingNode* root) const { assert(0); return 0; /*TO-DO*/}

protected:
	virtual std::string methodName() const { return "OS"; }

private:
	double computeExploitability(const BettingNode* node, const Hand hand, const int position)const { assert(0); return 0; /*TO-DO*/ };
};

double battle(const Game* game, const BettingNode* root, const CFR* p1, const CFR* p2, const int round);
double battle(const Game* game, const BettingNode* root, const VanillaCfr_RPB* p1, const VanillaCfr_RPB* p2, const int round);

#endif // !CFR_FAMILY_H
