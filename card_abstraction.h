#pragma once
#ifndef CARD_ABSTRACTION_H
#define CARD_ABSTRACTION_H

extern "C" {
#include "acpc_code/game.h"
}

#include "betting_node.h"
#include "FiveCardBucket.h"
#include "const.h"

class CardAbstraction {
public:

	CardAbstraction();
	virtual ~CardAbstraction();

	virtual int numBuckets(const Game* game, const BettingNode* node) const = 0;
	virtual int numBuckets(const Game* game, const State& state) const = 0;
	virtual int getBucket(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const = 0;

	virtual void getBucketForAllHand(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS],std::vector<int>& buckets)const = 0;
	virtual void getBucketForAllHand(const Game* game, const int round,
		const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const = 0;

	virtual void getBucketForAllHand(const Game* game, const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>[4]) const = 0;

	virtual void getDefeatedHand(const Game* game, const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_card[MAX_HOLE_CARDS], std::vector<int>& defeated) const;

	virtual bool canPrecomputeBuckets() const { return false; }
	virtual void precomputeBuckets(const Game* game, Hand& hand) const { };
	virtual void precomputeBuckets(const Game* game, Hand& hand, const int position) const { };

protected:
	static const int index_to_card_one[1326];
	static const int index_to_card_two[1326];
};

class EHS_Bucketing : public CardAbstraction {
public:

	EHS_Bucketing(const int num_buckets[MAX_ROUNDS]);
	virtual ~EHS_Bucketing();

	virtual int numBuckets(const Game* game, const BettingNode* node) const;
	virtual int numBuckets(const Game* game, const State& state) const;
	virtual int getBucket(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;

	virtual void getBucketForAllHand(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const;

	virtual void getBucketForAllHand(const Game* game, const int round,
		const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const;

	virtual void getBucketForAllHand(const Game* game, const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int> buckets[4]) const;

	virtual bool canPrecomputeBuckets() const;
	virtual void precomputeBuckets(const Game* game, Hand& hand) const;
	virtual void precomputeBuckets(const Game* game, Hand& hand, const int position) const;

protected:

	virtual int getBucket_preflop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;
	virtual int getBucket_flop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;
	virtual int getBucket_turn(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;
	virtual int getBucket_river(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;

	
	virtual void getBucketAll_flop(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const;
	virtual void getBucketAll_turn(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const;

	virtual void getBucketAll_river(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const;
	virtual void getBucketAll_preflop(const int8_t board_cards[MAX_BOARD_CARDS],std::vector<int>& buckets) const;

	int m_num_buckets[MAX_ROUNDS];

	
};

class RealProbBucketing_train : public CardAbstraction{
public:

	RealProbBucketing_train();
	virtual ~RealProbBucketing_train();

	virtual int numBuckets(const Game* game, const BettingNode* node) const { return 10; }
	virtual int numBuckets(const Game* game, const State& state) const { return 10; }
	virtual bool canPrecomputeBucket() const { return true; }
	virtual int getBucket(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const {
		assert(false);
		return 0;
	};

	void precomputeBuckets(Hand& hand, int prob[MAX_PLAYERS]) const;
	virtual void precomputeBuckets(const Game* game, Hand& hand) const { assert(false); }
};

/*class uneven_EHS : public CardAbstraction {
public:

	uneven_EHS(vector<double> m_bucket_density[MAX_ROUNDS]);
	virtual ~uneven_EHS();

	virtual int numBuckets(const Game* game, const BettingNode* node) const;
	virtual int numBuckets(const Game* game, const State& state) const;
	virtual int getBucket(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;

	virtual void getBucketForAllHand(const Game* game, const BettingNode* node,
		const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const;
	virtual void getBucketForAllHand(const Game* game, const int round,
		const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets)const;

	virtual void precomputeBuckets(const Game* game, Hand& hand) const { };
	virtual void precomputeBuckets(const Game* game, Hand& hand, const int position) const { };

protected:

	std::vector<double> m_bucket_density[MAX_ROUNDS];
	int m_num_buckets[MAX_ROUNDS];

private:

	//get the ES rank in all card.The former is the rank , the latter is the total card set.
	std::pair<int,int> getRankIn(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position)const;
	int getAllRank(const int round, const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& hand) const;
};*/

class unevenEHS : public EHS_Bucketing {
public:
	unevenEHS(const int num_buckets[MAX_ROUNDS],vector<double> m_bucket_density[MAX_ROUNDS]);

protected:
	virtual void getBucketAll_preflop(const int8_t board_cards[MAX_BOARD_CARDS], std::vector<int>& buckets) const;
	virtual int getBucket_preflop(const int8_t board_cards[MAX_BOARD_CARDS], const int8_t hole_cards[MAX_PLAYERS][MAX_HOLE_CARDS], const int position) const;
private:

	int preflopRankToBucket(int rank) const;

	std::vector<double> m_bucket_density[MAX_ROUNDS];
	int m_num_buckets[MAX_ROUNDS];
};

#endif // !CARD_ABSTRACTION_H
