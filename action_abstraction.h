#pragma once
#ifndef ACTION_ABSTRACTION_H
#define ACTION_ABSTRACTION_H

#define MAX_ABSTRACT_ACTIONS 4

#include "acpc_code/game.h"
using namespace project_acpc_server;



class ActionAbstraction {
public:

	ActionAbstraction();
	virtual ~ActionAbstraction();

	virtual int getActions(const Game* game,
		const State& state,
		Action actions[MAX_ABSTRACT_ACTIONS]) const = 0;
};

class NullActionAbstraction :public ActionAbstraction {
public:

	NullActionAbstraction();
	virtual ~NullActionAbstraction();

	virtual int getActions(const Game* game,
		const State& state,
		Action actions[MAX_ABSTRACT_ACTIONS]) const;
};

#endif // !ACTION_ABSTRACTION_H
