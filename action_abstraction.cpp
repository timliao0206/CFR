#include <assert.h>
#include <iostream>
#include "action_abstraction.h"

ActionAbstraction::ActionAbstraction()
{
	for (int i = 0; i < MAX_ROUNDS; i++) {
		max_raise_time[i] = MAX_NUM_ACTIONS;
	}
}

ActionAbstraction::~ActionAbstraction() {

}

int ActionAbstraction::getMaxRound(int round) const {
	assert(MAX_ROUNDS > round);

	return max_raise_time[round];
}

NullActionAbstraction::NullActionAbstraction(){
	
}

NullActionAbstraction::~NullActionAbstraction() {}

int NullActionAbstraction::getActions(const Game* game,
	const State& state,
	Action actions[MAX_ABSTRACT_ACTIONS]) const {

	int num_action = 0;
	bool error = false;

	for (int a = 0; a < NUM_ACTION_TYPES; a++) {
		Action action;
		action.type = (ActionType)a;
		action.size = 0;

		if (action.type == a_raise) {
			int32_t min_raise_size = 0;
			int32_t max_raise_size = 0;

			if (raiseIsValid(game, &state, &min_raise_size, &max_raise_size)) {
				if (num_action + (max_raise_size - min_raise_size) + 1 > MAX_ABSTRACT_ACTIONS) {
					error = true;
					break;
				}

				for (int s = min_raise_size; s <= max_raise_size; s++) {
					actions[num_action] = action;
					actions[num_action].size = s;

					++num_action;
				}
			}
		}
		else if (isValidAction(game, &state, 0, &action)) {
			if (num_action >= MAX_ABSTRACT_ACTIONS) {
				error = true;
				break;
			}

			actions[num_action] = action;
			++num_action;
		}
	}

	assert(!error);

	return num_action;
}

PropotionalActionAbstraction::PropotionalActionAbstraction(const std::array<int,MAX_ROUNDS> max_raise_t, int num_kind_of_size) :
	num_kind(num_kind_of_size), ActionAbstraction(){

	for (int i = 0; i < MAX_ROUNDS; i++) {
		max_raise_time[i] = max_raise_t[i];
	}

}

PropotionalActionAbstraction::~PropotionalActionAbstraction() {

}

int PropotionalActionAbstraction::getActions(const Game* game, const State& state, Action actions[MAX_ABSTRACT_ACTIONS]) const{

	int num_actions = 0;
	bool error = false;

	for (int i = 0; i < NUM_ACTION_TYPES; i++) {
		Action action;
		action.type = (ActionType)i;
		action.size = 0;

		if (action.type == a_raise) {

			int32_t min_raise_size = 0;
			int32_t max_raise_size = 0;

			if (raiseIsValid(game, &state, &min_raise_size, &max_raise_size)) {

				assert(num_actions + num_kind < MAX_ABSTRACT_ACTIONS);

				const double interval = (double)(max_raise_size - min_raise_size) / (double)(num_kind - 1.0);

				double accumulation = min_raise_size;

				for (int size = 0; size < num_kind; size++) {
					actions[num_actions] = action;
					actions[num_actions].size = (int)accumulation;

					accumulation += interval;
					num_actions++;
				}
			}
		}else if (isValidAction(game, &state, 0, &action)) {
			assert(num_actions < MAX_ABSTRACT_ACTIONS);

			actions[num_actions] = action;
			++num_actions;
		}
	}

	return num_actions;
}
