#include "CFR_family.h"
#define ITERATION_TIMES 10000
#define EXPLOITABILITY_MONTE_ITERATE_TIMES 100
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std::chrono;

int CFR::getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const {

	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->getStrategy(index, bucket, node->getNumAction(), strategy);
}

int CFR::getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const {
	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->getAverageStrategy(index, bucket, node->getNumAction(), strategy);
}

void CFR::updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret) {

	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	stored[node->getRound()]->updateRegret(index, bucket, regret);
}

void CFR::updateStrategySum(const BettingNode* node, const Hand hand, const int position, std::vector<double> strategy) {
	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	stored[node->getRound()]->updateStrategySum(index, bucket, strategy);
}

int CFR::sampleAction(const BettingNode* node, const Hand hand, const int position) const{

	int64_t index = node->getIndex();
	int bucket = 0;

	if (card_abs->canPrecomputeBuckets())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->sampleAction(index, bucket, node->getNumAction());
}

void CFR::printRegretSum(std::string fileName) const{
	std::fstream file;
	file.open(fileName, std::ios::out);

	if (!file) {
		assert(0);
	}

	file << "method = " + methodName() + "\n";
	file << "data = regret_sum\n";
	file << "size = ";
	for (int i = 0; i < game->numRounds; i++) {
		file << stored[i]->getRegretSumSize() << " ";
	}
	file << "\n";

	for (int i = 0; i < game->numRounds; i++) {
		file << "START OF ROUND " << i << "\n";
		file.close();
		stored[i]->printRegretSum(fileName);
		file.open(fileName, std::ios::out | std::ios::app);
		file << "END OF ROUND " << i << "\n";
	}
	file.close();
	return;
}

void CFR::printStrategySum(std::string fileName) const{
	std::fstream file;
	file.open(fileName, std::ios::out);

	if (!file) {
		assert(0);
	}

	file << "method = " + methodName() + "\n";
	file << "data = strategy_sum\n";
	file << "size = ";
	for (int i = 0; i < game->numRounds; i++) {
		file << stored[i]->getStrategySumSize() << " ";
	}
	file << "\n";

	for (int i = 0; i < game->numRounds; i++) {
		file << "START OF ROUND " << i << "\n\n";
		file.close();
		stored[i]->printStrategySum(fileName);
		file.open(fileName, std::ios::out | std::ios::app);
		file << "END OF ROUND " << i << "\n";
	}
	file.close();
	return;
}

void CFR::readFile(std::string fileName) {

	std::fstream file;
	file.open(fileName, std::ios::in | std::ios::app);

	char buffer[200];

	file.getline(buffer, sizeof(buffer));
	std::string method = methodName();
	method = "method = " + method;
	if (strncmp(buffer, method.c_str(), sizeof(method.c_str()))) {
		std::cout << "Invalid File : method inconsistent\n";
		return;
	}

	file.getline(buffer, sizeof(buffer));
	if (!strncmp(buffer, "data = regret_sum", 17)) {

		file.read(buffer, 6);
		if (!strncmp(buffer, "size =", 6)) {
			for (int i = 0; i < game->numRounds; i++) {
				long size;
				file >> size;
				stored[i]->resizeRegretSum(size);
			}
		}
		file.getline(buffer, sizeof(buffer));
		int round = -1;
		while (file.getline(buffer, sizeof(buffer))) {
			if (!strncmp(buffer, "START OF ROUND", 14)) {
				round++;
				continue;
			}

			if (!strncmp(buffer, " END OF ROUND", 13)) {
				continue;
			}

			int64_t index;
			double data;
			file >> index >> data;
			stored[round]->writeRegretSum(index, data);
		}
		file.close();
	}
	else if (!strncmp(buffer, "data = strategy_sum", 19)) {
		file.read(buffer, 6);
		if (!strncmp(buffer, "size =", 6)) {
			for (int i = 0; i < game->numRounds; i++) {
				long size;
				file >> size;
				stored[i]->resizeStrategySum(size);
			}
		}
		file.getline(buffer, sizeof(buffer));
		int round = -1;
		while (file.getline(buffer, sizeof(buffer))) {
			if (!strncmp(buffer, "START OF ROUND", 14)) {
				round++;
				continue;
			}

			if (!strncmp(buffer, " END OF ROUND", 13)) {
				continue;
			}

			int64_t index;
			double data;
			file >> index >> data;
			stored[round]->writeStrategySum(index, data);
		}
		file.close();
	}
	else {
		std::cout << "Invalid File : undefine data type\n";
		return;
	}
}

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

void VanillaCfr::doIteration(const BettingNode* root , const int times) {
	for (int i = 0; i < times; i++) {
		//long long a = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		Hand hand = generateHand(game);
		card_abs->precomputeBuckets(game, hand);
		//long long b = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		//bucket += (b - a);
		for (int player = 0; player < game->numPlayers; player++) {
			walkTree(player, root, hand, 1.0, 1.0);
			long long c = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			//cfr += (c - b);
		}
	}
	//std::cout << bucket << std::endl;
	//std::cout << cfr << std::endl;
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
		if (!player)
			value[i] = this->walkTree(position, child, hand, strategy[i] * prob_p1, prob_p2);
		else
			value[i] = this->walkTree(position, child, hand, prob_p1, strategy[i] * prob_p2);

		child = child->getSibiling();

		return_value += value[i] * strategy[i];
	}

	if (position == player) {
		for (int i = 0; i < num_choices; i++) {
			regret[i] = (value[i] - return_value) * (position == 0 ? prob_p2 : prob_p1);
		}

		this->updateRegret(cur_node, hand, position, regret);
	}

	this->updateStrategySum(cur_node, hand, position, strategy);

	return return_value;
}

//if R+ = 0, return 1
int VanillaCfr::getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const{

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

double VanillaCfr::computeExploitability(const BettingNode* node, const Hand hand, const int position) const {

	if (node->getChild() == NULL) {
		return node->evaluate(hand, position);
	}

	int num_actions = node->getNumAction();

	if (node->getPlayer() == position) {
		double max_value = -DBL_MAX + 1;
		node = node->getChild();
		for (int a = 0; a < num_actions; a++) {
			double ex_value = computeExploitability(node, hand, position);
			max_value = ex_value > max_value ? ex_value : max_value;

			node = node->getSibiling();
		}
		return max_value;
	}
	else {
		double sum = 0;

		std::vector<double> strategy(num_actions);
		getStrategy(node, hand, !position, strategy);

		node = node->getChild();
		for (int a = 0; a < num_actions; a++) {

			if (strategy[a] != 0) {
				sum += strategy[a] * computeExploitability(node, hand, position);
			}
			//sum += 1.0 / (double)num_actions * computeExploitability(node, hand, position);
			node = node->getSibiling();
		}

		return sum;
	}
}

double VanillaCfr::getExploitability(const BettingNode* root) const {

	double sum = 0;
	for (int iter = 0; iter < EXPLOITABILITY_MONTE_ITERATE_TIMES; iter++) {
		Hand hand = generateHand(game);
		card_abs->precomputeBuckets(game, hand);

		for (int p = 0; p < game->numPlayers; p++) {
			sum += computeExploitability(root, hand, p) / EXPLOITABILITY_MONTE_ITERATE_TIMES;
		}
	}

	return sum;
}

//RealProb test
void VanillaCfr_RPB::doIteration(const BettingNode* root, const int times) {
	int prob[MAX_PLAYERS];

	for (int t = 0; t < times; t++) {
		for (int p1 = 0; p1 <= 10; p1++) {
			prob[0] = p1;
			for (int p2 = 0; p2 <= 10; p2++) {
				prob[1] = p2;

				Hand hand = generateEmptyHand();
				RPB->precomputeBucket(hand, prob);

				for (int player = 0; player < game->numPlayers; player++) {
					walkTree(player, root, hand, 1.0, 1.0);
				}
			}
		}
	}
}

double VanillaCfr_RPB::getExploitability(const BettingNode* root)const {
	int prob[MAX_PLAYERS];
	double sum = 0;
	for (int p1 = 0; p1 <= 10; p1++) {
		prob[0] = p1;
		for (int p2 = 0; p2 <= 10; p2++) {
			prob[1] = p2;

			Hand hand = generateEmptyHand();
			RPB->precomputeBucket(hand, prob);

			for (int player = 0; player < game->numPlayers; player++) {
				sum += computeExploitability(root, hand, player) / 242;
			}
		}
	}
	return sum;
}

double VanillaCfr_RPB::computeExploitability(const BettingNode* node, const Hand hand, const int position)const {
	if (node->getChild() == NULL) {
		/*int bucket_p1 = hand.precomputed_bucket[0][0];
		int bucket_p2 = hand.precomputed_bucket[0][1];

		double p1_win = 0;
		double p2_win = 0;

		if ((bucket_p1 != 0 || bucket_p2 != 0) && (bucket_p1 != 10 || bucket_p2 != 10)) {
			p1_win = (double)(bucket_p1) * (10 - bucket_p2) / ((bucket_p1) * (10 - bucket_p2) + (bucket_p2) * (10 - bucket_p1));
			p2_win = (double)(bucket_p2) * (10 - bucket_p1) / ((bucket_p1) * (10 - bucket_p2) + (bucket_p2) * (10 - bucket_p1));
		}
		else {
			return 0;
		}

		return (position ? -1 : 1) * ( (p1_win - p2_win) * (node->getPot()) );*/

		return node->evaluate(hand, position);
	}

	int num_actions = node->getNumAction();

	if (node->getPlayer() == position) {
		double max_value = -DBL_MAX + 1;
		node = node->getChild();
		for (int a = 0; a < num_actions; a++) {
			double ex_value = computeExploitability(node, hand, position);
			max_value = ex_value > max_value ? ex_value : max_value;

			node = node->getSibiling();
		}
		return max_value;
	}
	else {
		double sum = 0;

		std::vector<double> strategy(num_actions);
		getAverageStrategy(node, hand, !position, strategy);

		node = node->getChild();
		for (int a = 0; a < num_actions; a++) {

			if (strategy[a] != 0) {
				sum += strategy[a] * computeExploitability(node, hand, position);
			}
			//sum += 1.0 / (double)num_actions * computeExploitability(node, hand, position);
			node = node->getSibiling();
		}

		return sum;
	}
}

VanillaCfr_RPB::VanillaCfr_RPB(const Game* new_game, const RealProbBucketing_train* new_RPB, size_t num_entries_per_bucket[MAX_ROUNDS]) {
	for (int j = 0; j < MAX_ROUNDS; j++) {
		if (j >= new_game->numRounds) {
			stored[j] = NULL;
		}
		else {
			stored[j] = new Storage(num_entries_per_bucket[j]);
		}
	}

	RPB = new_RPB;
	card_abs = NULL;
	game = new_game;
}

int VanillaCfr_RPB::getStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const {

	int64_t index = node->getIndex();
	int bucket = 0;

	if (RPB->canPrecomputeBucket())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->getStrategy(index, bucket, node->getNumAction(), strategy);
}

int VanillaCfr_RPB::getAverageStrategy(const BettingNode* node, const Hand hand, const int position, std::vector<double>& strategy) const {
	int64_t index = node->getIndex();
	int bucket = 0;

	if (RPB->canPrecomputeBucket())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	return stored[node->getRound()]->getAverageStrategy(index, bucket, node->getNumAction(), strategy);
}

void VanillaCfr_RPB::updateRegret(const BettingNode* node, const Hand hand, const int position, std::vector<double> regret) {

	int64_t index = node->getIndex();
	int bucket = 0;

	if (RPB->canPrecomputeBucket())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	stored[node->getRound()]->updateRegret(index, bucket, regret);
}

void VanillaCfr_RPB::updateStrategySum(const BettingNode* node, const Hand hand, const int position, std::vector<double> strategy) {
	int64_t index = node->getIndex();
	int bucket = 0;

	if (RPB->canPrecomputeBucket())
		bucket = hand.precomputed_bucket[position][node->getRound()];
	else
		bucket = card_abs->getBucket(game, node, hand.board_cards, hand.hole_cards, position);

	stored[node->getRound()]->updateStrategySum(index, bucket, strategy);
}

double VanillaCfr_RPB::walkTree(const int position, const BettingNode* cur_node, const Hand hand, double prob_p1, double prob_p2) {

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
		if (!player)
			value[i] = this->walkTree(position, child, hand, strategy[i] * prob_p1, prob_p2);
		else
			value[i] = this->walkTree(position, child, hand, prob_p1, strategy[i] * prob_p2);

		child = child->getSibiling();

		return_value += value[i] * strategy[i];
	}

	if (position == player) {
		for (int i = 0; i < num_choices; i++) {
			regret[i] = (value[i] - return_value) * (position == 1 ? prob_p1 : prob_p2);
		}

		this->updateRegret(cur_node, hand, position, regret);
		this->updateStrategySum(cur_node, hand, position, strategy);
	}

	return return_value;
}

void VanillaCfr_RPB::doIteration(const BettingNode* root) {
	doIteration(root, 1);
}


//END of RPB

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
		for (int p = 0; p < game->numPlayers; p++) {
			Hand hand = generateHand(game);
			card_abs->precomputeBuckets(game, hand);
			walkTree(p, root, hand);
		}
	}
}

void ES::doIteration(const BettingNode* root, const int time) {

	for (int i = 0; i < time; i++) {
		for (int p = 0; p < game->numPlayers; p++) {
			Hand hand = generateHand(game);
			card_abs->precomputeBuckets(game, hand);
			walkTree(p, root, hand);
		}
	}
}

double ES::walkTree(const int position, const BettingNode* cur_node, const Hand hand) {

	go_through++;

	//terminal node
	if (cur_node->getChild() == NULL) {
		return cur_node->evaluate(hand, position);
	}

	if (cur_node->getPlayer() != position) {
		int sampledAction = sampleAction(cur_node, hand, !position);

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

double ES::computeExploitability(const BettingNode* node, const Hand hand, const int position) const{

	if (node->getChild() == NULL) {
		return node->evaluate(hand, position);
	}

	int num_actions = node->getNumAction();

	if (node->getPlayer() == position) {
		double max_value = -DBL_MAX + 1;
		node = node->getChild();
		for (int a = 0; a < num_actions; a++) {
			double ex_value = computeExploitability(node, hand, position);
			max_value = ex_value > max_value ? ex_value : max_value;

			node = node->getSibiling();
		}
		return max_value;
	}
	else {
		double sum = 0;

		std::vector<double> strategy(num_actions);
		getStrategy(node, hand, !position, strategy);

		node = node->getChild();
		for (int a = 0; a < num_actions; a++) {

			if (strategy[a] != 0) {
				sum += strategy[a] * computeExploitability(node, hand, position);
			}
			//sum += 1.0 / (double)num_actions * computeExploitability(node, hand, position);
			node = node->getSibiling();
		}

		return sum;
	}
}

double ES::getExploitability(const BettingNode* root) const {

	double sum = 0;
	for (int iter = 0; iter < EXPLOITABILITY_MONTE_ITERATE_TIMES; iter++) {
		Hand hand = generateHand(game);
		card_abs->precomputeBuckets(game, hand);

		for (int p = 0; p < game->numPlayers; p++) {
			sum += computeExploitability(root, hand, p) / EXPLOITABILITY_MONTE_ITERATE_TIMES;
		}
	}

	return sum;
}

double battle(const Game* game, const BettingNode* root, const CFR* p1, const CFR* p2, const int round) {
	double sum = 0;

	for (int i = 0; i < round; i++) {
		Hand hand = generateHand(game);
		p1->getCardAbstraction()->precomputeBuckets(game, hand,0);
		p2->getCardAbstraction()->precomputeBuckets(game, hand, 1);

		const BettingNode* current = root;

		while (current ->getChild() != nullptr){
			if (current->getPlayer())
				current = current->doAction(p2->sampleAction(current, hand, 1));
			else
				current = current->doAction(p1->sampleAction(current, hand, 0));
		}

		sum += current->evaluate(hand, 0) / round;

		p1->getCardAbstraction()->precomputeBuckets(game, hand, 1);
		p2->getCardAbstraction()->precomputeBuckets(game, hand, 0);

		current = root;

		while (current->getChild() != nullptr) {
			if (!current->getPlayer())
				current = current->doAction(p2->sampleAction(current, hand, 1));
			else
				current = current->doAction(p1->sampleAction(current, hand, 0));
		}

		sum += current->evaluate(hand, 1) / round;
	}

	return sum;
}