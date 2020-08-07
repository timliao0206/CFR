#include <assert.h>

#include "betting_node.h"


BettingNode::BettingNode() {
	sibling = nullptr;
}

BettingNode::~BettingNode() {

}

TerminalNode::TerminalNode(const bool new_showdown, const int8_t new_fold[2], const int16_t new_pot)
  : BettingNode(),
	showdown(new_showdown),
	pot(new_pot)
{
	fold[0] = new_fold[0];
	fold[1] = new_fold[1];
}

TerminalNode::~TerminalNode(){}

double TerminalNode::evaluate(const Hand& hand, const int position) const{

	return (showdown ? hand.show_down_value[position]
		: fold[position]) * (double)pot;
}

InfoSet::InfoSet(const int64_t new_index,
	const int8_t new_num_choice,
	const int8_t new_player,
	const int8_t new_round,
	const BettingNode* new_child) 
	:index(new_index),
	num_choice(new_num_choice),
	player(new_player),
	round(new_round),
	child(new_child)
{
}

InfoSet::~InfoSet() {

}

const BettingNode* InfoSet::doAction(const int action) const {

	assert(action < num_choice);
	const BettingNode* cur = this -> child;
	for (int i = 0; i < action; i++) {
		cur = cur->getSibiling();
	}

	return cur;
}

int64_t BettingNode::size;

BettingNode* initBettingTree(State& state,
	const Game* game,
	const ActionAbstraction* action_abstraction,
	size_t num_entries_per_bucket[MAX_ROUNDS],
	int8_t raise_time) {

	BettingNode::size++;

	if (state.finished) {//terminal node

		bool fold = state.playerFolded[0] || state.playerFolded[1];

		int8_t fold_value[2];
		int32_t money;

		if (fold) {
			fold_value[0] = state.playerFolded[0] != 0 ? -1 : 1;
			fold_value[1] = state.playerFolded[1] != 0 ? -1 : 1;
			money = state.spent[(state.playerFolded[0] != 0 ? 0 : 1)];
		}
		else {
			fold_value[0] = 0;
			fold_value[1] = 0;
			money = state.spent[0];
		}

		TerminalNode* node = new TerminalNode(!fold, fold_value, money);

		return node;
	}

	Action actions[MAX_ABSTRACT_ACTIONS];
	int num_action = action_abstraction->getActions(game, state, actions);

	int64_t index = num_entries_per_bucket[state.round];

	num_entries_per_bucket[state.round] += num_action;

	BettingNode* first = NULL;
	BettingNode* last = NULL;

	for (int a = 0; a < num_action; a++) {

		if (a >= 2 && raise_time >= action_abstraction->getMaxRound(state.round)) break;

		State tmp_state(state);
		doAction(game, &actions[a], &tmp_state);

		BettingNode* child = initBettingTree(tmp_state, game, action_abstraction, num_entries_per_bucket,( a < 2 ? 0 : raise_time + 1));

		if (first == NULL) {
			first = child;
		}
		
		if (last != NULL) {
			last->setSibling(child);
		}

		last = child;
	}

	assert(first != NULL && last != NULL);

	last->setSibling(NULL);

	InfoSet* node = new InfoSet(index, num_action, currentPlayer(game, &state), state.round, first);

	return node;
}
