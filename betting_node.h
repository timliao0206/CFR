#pragma once

#ifndef BETTING_NODE_H
#define BETTING_NODE_H


#include <inttypes.h>

#include "acpc_code/game.h"
#include "action_abstraction.h"
#include "hand.h"
#include <assert.h>

class BettingNode {
public:

	BettingNode();
	virtual ~BettingNode();

	virtual const BettingNode* getChild() const = 0;
	virtual const BettingNode* getSibiling() const { return sibling; }
	virtual void setSibling(BettingNode* new_sibling) { sibling = new_sibling; }

	virtual double evaluate(const Hand& hand, const int position) const { assert(0); return 0; };
	virtual int8_t getNumAction() const { assert(0); return 0; };
	virtual int64_t getIndex() const { assert(0); return 0; }
	virtual int8_t getPlayer() const { assert(0); return 0; }
	virtual int8_t getRound() const { assert(0); return 0; }
	virtual int8_t did_player_fold(const int position) const { assert(0); return 0; }

protected:
	BettingNode* sibling;
	
};

class TerminalNode: public BettingNode{
public:

	TerminalNode(const bool new_showdown, const int8_t new_fold[2] , const int8_t new_pot);
	virtual ~TerminalNode();

	virtual BettingNode* getChild() const { return NULL; }

	virtual double evaluate(const Hand& hand, const int position) const;

protected:
	const bool showdown;
	int8_t fold[2];
	const int8_t pot;
};

class InfoSet: public BettingNode {
public:

	InfoSet(const int64_t new_index,
			const int8_t new_num_choice,
			const int8_t new_player,
			const int8_t new_round,
			const BettingNode* new_child);

	virtual ~InfoSet();

	virtual int64_t getIndex() const { return index; }
	virtual int8_t getNumAction() const { return num_choice; }
	virtual int8_t getPlayer() const { return player; }
	virtual int8_t getRound() const { return round; }

	virtual const BettingNode* getChild() const { return child; }

protected:
	const int64_t index;
	const int8_t num_choice;
	const int8_t player;
	const int8_t round;
	const BettingNode* child;
};

BettingNode* initBettingTree(State& state,
	const Game* game,
	const ActionAbstraction* action_abstraction,
	size_t num_entries_per_bucket[MAX_ROUNDS]
);



#endif 

