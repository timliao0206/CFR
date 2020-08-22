#pragma once
#ifndef ACTION_ABSTRACTION_H
#define ACTION_ABSTRACTION_H

#include "acpc_code/game.h"
#include "const.h"

#include <array>
using namespace project_acpc_server;



class ActionAbstraction {
public:

	ActionAbstraction();
	virtual ~ActionAbstraction();

	virtual int getActions(const Game* game,
		const State& state,
		Action actions[MAX_ABSTRACT_ACTIONS]) const = 0;

	virtual int getMaxRound(int round) const;

protected:

	std::array<int, MAX_ROUNDS> max_raise_time;
};

class NullActionAbstraction :public ActionAbstraction {
public:

	NullActionAbstraction();
	virtual ~NullActionAbstraction();

	virtual int getActions(const Game* game,
		const State& state,
		Action actions[MAX_ABSTRACT_ACTIONS]) const;
};

class PropotionalActionAbstraction : public ActionAbstraction {
public:

	PropotionalActionAbstraction(const std::array<int,MAX_ROUNDS> max_raise_t, int num_kind_of_size);
	virtual ~PropotionalActionAbstraction();
	virtual int getActions(const Game* game,
		const State& state,
		Action actions[MAX_ABSTRACT_ACTIONS]) const;

private:
	const int num_kind;
};

#endif // !ACTION_ABSTRACTION_H
