#pragma once
#ifndef CFR_FAMILY_H
#define CFR_FAMILY_H

#include "hand.h"
#include "betting_node.h"
#include "storage.h"
#include "card_abstraction.h"
#include "const.h"

#include <vector>
#include <string>
#include <queue>

extern "C" {
#include "acpc_code/game.h"
}

class CFR {
public:

	//do iterations of this CFR
	virtual void doIteration(const BettingNode* root) = 0;
	virtual void doIteration(const BettingNode* root, const int times) = 0;

	//compute this agent's exploitability (currently not working because of unknown reason)
	virtual double getExploitability(const BettingNode* root) const = 0;

	//Infomation Set mentioned below was comprised of a BettingNode and a Hand + PlayerPosition

	//get Strategy of given Infomation Set by regret-matching, returning values in strategy vector 
	virtual int getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	virtual int getStrategy(const BettingNode* node, const int bucket, std::vector<double>& strategy) const;

	//get AverageStrategy of given Infomation Set by averaging all previous strategy, returning value in strategy vector
	virtual int getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const;
	
	//update regret value of given Infomation Set
	virtual void updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret);

	//update strategy sum of given Infomation Set
	virtual void updateStrategySum(const BettingNode* node, const Hand hand, const int position, std::vector<double> strategy);
	
	//sample an action based on regret-matching-generated strategy
	virtual int sampleAction(const BettingNode* node, const Hand hand, const int position) const;

	//print regret sum into a file
	virtual void printRegretSum(std::string fileName) const;

	//print strategy sum into a file
	virtual void printStrategySum(std::string fileName) const;

	//get expected value of given Infomation Set
	virtual double expectedValue(const BettingNode* root, const std::queue<int> action_sequence,const BettingNode* node, const Hand hand, const int position) const;
	
	//get the probability of reaching given Infomation Set of all possible private hand
	virtual void getHandProbability(const BettingNode* root, const std::queue<int> action_sequence,const std::vector<int> bucket[4], std::vector<double>& return_value) const;

	//clear all the data in this object, and reset parameters.
	virtual void resetStorage(size_t num_entries_per_bucket[MAX_ROUNDS]);

	//read datas in the given file, 
	virtual void readFile(std::string fileName);

	//get the card abstraction pointer
	virtual const CardAbstraction* getCardAbstraction() const{ return card_abs; }

	//get the game pointer
	virtual const Game* getGame() const{ return game; }
protected:

	virtual double expectedValue(const BettingNode* node,const std::vector<int> buckets[4],std::vector<double> hand_probs,const std::vector<int> win_or_lose) const;

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

	virtual double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);

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
	virtual double walkTree(const int position, const BettingNode* cur_node, const Hand hand);

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

class VanillaCFR_fixedOpponent : public VanillaCfr {

public:
	VanillaCFR_fixedOpponent(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs, const CFR* opponent);
	~VanillaCFR_fixedOpponent();
	virtual double walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2);

protected:

	//only being used to obtain staretgy
	const CFR* opponent;
};

class ES_fixedOpponent : public ES {
public:
	ES_fixedOpponent(const Game* game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs, const CFR* oppo);
	~ES_fixedOpponent();
	
	virtual int sampleAction(const BettingNode* node, const Hand hand, const int position) const;

protected:
	const CFR* opponent;
};

double battle(const Game* game, const BettingNode* root, const CFR* p1, const CFR* p2, const int round);
double battle(const Game* game, const BettingNode* root, const VanillaCfr_RPB* p1, const VanillaCfr_RPB* p2, const int round);

//This function compute the similarity between two CFR modual.
//It only consider the preflop and flop turn,since the other two's strategies can be computed real-time.
//The value ranges from 0 to 1.
double computeSimilarity(const CFR* p1, const CFR* p2,const BettingNode* root);
#endif // !CFR_FAMILY_H
