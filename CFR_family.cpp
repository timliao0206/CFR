#include "CFR_family.h"
#define ITERATION_TIMES 100
#include <vector>

VanillaCfr::VanillaCfr(const Game* new_game, size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs) {

	
	for (int j = 0; j < MAX_ROUNDS; j++) {
		if (j >= new_game->numRounds) {
			stored[j] = NULL;
		}
		else {
			stored[j] = new Storage(num_entries_per_bucket[j]);
		}
	}
	
	card_abs = abs;
	game = new_game;
}

VanillaCfr::~VanillaCfr(){}


int VanillaCfr::go_through;

void VanillaCfr::doIteration(const BettingNode* root) {
	for (int i = 0; i < ITERATION_TIMES; i++) {
		Hand hand = generateHand(game);
		card_abs->precomputeBuckets(game, hand);
		walkTree(0, root, hand,1.0,1.0);
	}
}

double VanillaCfr::walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2) {

	go_through++;

	//terminal node
	if (cur_node->getChild() == NULL) {
		return cur_node->evaluate(hand, position);
	}

	const int num_choices = cur_node->getNumAction();
	const int player = cur_node->getPlayer();

	std::vector<double> value(num_choices);
	std::vector<double> strategy(num_choices);
	std::vector<double> regret(num_choices);

	this->getStrategy(cur_node, hand, position, strategy);

	double return_value = 0;

	const BettingNode* child(cur_node->getChild());
	for (int i = 0; i < num_choices; i++) {
		if (player)
			value[i] = this->walkTree(position, child, hand, strategy[i] * prob_p1, prob_p2);
		else
			value[i] = this->walkTree(position, child, hand, prob_p1, strategy[i] * prob_p2);

		child = child->getSibiling();

		return_value += value[i] * strategy[i];
	}

	if (position == player) {
		for (int i = 0; i < num_choices; i++) {
			regret[i] = (value[i] - return_value) * (position == 1 ? prob_p2 : prob_p1);
		}

		this->updateRegret(cur_node, hand, position, regret);
	}

	return return_value;
}

//if R+ = 0, return 1
int VanillaCfr::getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) {

	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->getStrategy(index, bucket, node->getNumAction(), strategy);
}

void VanillaCfr::updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret) {

	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	stored[node->getRound()]->updateRegret(index, bucket, regret);
}

ES::ES(const Game* new_game,size_t num_entries_per_bucket[MAX_ROUNDS], const CardAbstraction* abs) {

	for (int j = 0; j < MAX_ROUNDS; j++) {
		if (j >= new_game->numRounds) {
			stored[j] = NULL;
		}
		else {
			stored[j] = new Storage(num_entries_per_bucket[j]);
		}
	}

	card_abs = abs;
	game = new_game;
}

ES::~ES(){}

int ES::go_through;

void ES::doIteration(const BettingNode* root) {

	for (int i = 0; i < ITERATION_TIMES; i++) {
		Hand hand = generateHand(game);
		card_abs->precomputeBuckets(game, hand);
		walkTree(0, root, hand);
	}
}

double ES::walkTree(const int position, const BettingNode* cur_node, const Hand hand) {

	go_through++;

	//terminal node
	if (cur_node->getChild() == NULL) {
		return cur_node->evaluate(hand, position);
	}

	if (cur_node->getPlayer() != position) {
		int sampledAction = sampleAction(cur_node, hand, position);

		const BettingNode* child = cur_node->getChild();
		for (int i = 0; i < sampledAction; i++) {
			child = child->getSibiling();
		}

		return walkTree(position, child, hand);
	}

	int num_choices = cur_node->getNumAction();

	double return_value = 0;

	std::vector<double> values(num_choices);
	std::vector<double> strategy(num_choices);
	std::vector<double> regret(num_choices);

	getStrategy(cur_node, hand, position, strategy);

	const BettingNode* child = cur_node->getChild();
	for (int i = 0; i < num_choices; i++) {
		values[i] = walkTree(position, child, hand);

		return_value += values[i] * strategy[i];
		child = child->getSibiling();
	}

	for (int i = 0; i < num_choices; i++) {
		regret[i] = values[i] - return_value;
	}

	updateRegret(cur_node, hand,position,regret);

	return return_value;
}

int ES::getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) {
	
	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards,position);

	return stored[node->getRound()] -> getStrategy(index, bucket, node->getNumAction(), strategy);
}

void ES::updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret) {
	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	stored[node->getRound()] -> updateRegret(index,bucket,regret);
}

int ES::sampleAction(const BettingNode* node, const Hand hand, const int position) {
	
	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->sampleAction(index, bucket, node->getNumAction());
}
