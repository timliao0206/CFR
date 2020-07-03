#include <assert.h>
#include "action_abstraction.h"

ActionAbstraction::ActionAbstraction()
{

}

ActionAbstraction::~ActionAbstraction() {

}

NullActionAbstraction::NullActionAbstraction(){}

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
