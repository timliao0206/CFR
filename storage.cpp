#include "storage.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Storage::Storage(size_t per_bucket):num_entries_per_bucket(per_bucket) {
	regret_sum.clear();
	strategy_sum.clear();
}

Storage::~Storage() {
	
}

int64_t Storage::getLocalIndex(const int64_t index, const int bucket) const{

	return ((int64_t)num_entries_per_bucket * bucket) + index;
}

int Storage::getStrategy(const int64_t index, const int bucket, const int num_action, std::vector<double>& strategy){

	int64_t localIndex = getLocalIndex(index, bucket);

	if (localIndex + num_action >= regret_sum.size()) {

		assert(localIndex + num_action < regret_sum.max_size());

		regret_sum.resize(localIndex + num_action,0);

		for (int i = 0; i < num_action; i++) {
			strategy[i] = 1.0 / (double)num_action;
		}
		return -1;
	}

	strategy.resize(num_action);

	double sum = 0;
	for (int i = 0; i < num_action; i++) {
		sum += (regret_sum[localIndex + i] > 0) ? regret_sum[localIndex + i] : 0;
	}

	if (sum <= 0) {
		for (int i = 0; i < num_action; i++) {
			strategy[i] = 1.0 /(double) num_action;
		}

		return 0;
	}

	for (int i = 0; i < num_action; i++) {
		strategy[i] = ( (regret_sum[localIndex + i] > 0) ? regret_sum[localIndex + i] / sum : 0 );
	}

	return 1;
}

int Storage::getAverageStrategy(const int64_t index, const int bucket, const int num_action, std::vector<double>& strategy) {

	int64_t localIndex = getLocalIndex(index, bucket);

	if (localIndex + num_action >= strategy_sum.size()) {

		assert(localIndex + num_action < strategy_sum.max_size());

		strategy_sum.resize(localIndex + num_action, 0);

		for (int i = 0; i < num_action; i++) {
			strategy[i] = 1.0 / (double)num_action;
		}
		return -1;
	}

	strategy.resize(num_action);

	double sum = 0;
	for (int i = 0; i < num_action; i++) {
		sum += strategy_sum[localIndex + i];
	}

	if (sum <= 0) {
		for (int i = 0; i < num_action; i++) {
			strategy[i] = 1.0 / (double)num_action;
		}

		return 0;
	}

	for (int i = 0; i < num_action; i++) {
		strategy[i] = strategy_sum[i] / sum;
	}

	return 1;
}

void Storage::updateRegret(const int64_t index, const int bucket, std::vector<double> regret) {

	int64_t localIndex = getLocalIndex(index, bucket);
	int num_action = regret.size();

	if (localIndex + num_action > regret_sum.size()) {

		assert(localIndex + num_action < regret_sum.max_size());
		regret_sum.resize(localIndex + num_action, 0);
	}

	for (int i = 0; i < num_action; i++) {
		regret_sum[localIndex + i] += regret[i];
	}

	return;
}

void Storage::updateStrategySum(int64_t index, const int bucket, std::vector<double> strategy) {
	int64_t localIndex = getLocalIndex(index, bucket);
	int num_action = strategy.size();

	if (localIndex + num_action > strategy_sum.size()) {

		assert(localIndex + num_action < strategy_sum.max_size());
		strategy_sum.resize(localIndex + num_action, 0);
	}

	for (int i = 0; i < num_action; i++) {
		strategy_sum[localIndex + i] += strategy[i];
	}

	return;
}

int Storage::sampleAction(const int64_t index, const int bucket, const int num_action){

	int64_t localIndex = getLocalIndex(index, bucket);

	std::vector<double> strategy(num_action);
	int status = getStrategy(index, bucket, num_action, strategy);

	double ran = (double)rand() / (double)(RAND_MAX + 1.0);

	if (status == -1) {
		int sampled = 0;
		while (ran >= 0) {
			ran -= 1.0/(double)num_action;
			sampled++;

			assert(sampled <= num_action);
		}
		sampled--;

		return sampled;
	}

	double sum = 0;
	for (int i = 0; i < num_action; i++) {
		sum += strategy[i];
	}

	int sampled = 0;
	while (ran > 0) {
		assert(sampled < num_action);

		ran -= strategy[sampled] / sum;
		sampled++;
	}
	sampled--;

	return sampled;
}

void Storage::printRegretSum(std::string fileName) {

	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::app);
	for (int i = 0; i < regret_sum.size(); i++) {
		if(i == regret_sum.size() - 1)
			file << i << " " << regret_sum[i] << " ";
		else
			file << i << " " << regret_sum[i] << "\n";
	}
	file.close();
	return;
}

void Storage::printStrategySum(std::string fileName) {

	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::app);
	for (int i = 0; i < strategy_sum.size(); i++) {
		if(i == strategy_sum.size() - 1)
			file << i << " " << strategy_sum[i] << " ";
		else
			file << i << " " << strategy_sum[i] << "\n";
	}
	file.close();
	return;
}

void Storage::writeRegretSum(int64_t index, double data) {
	assert(index + 1 < regret_sum.max_size());
	if (index >= regret_sum.size()) {
		regret_sum.resize(index + 1,0);
	}

	regret_sum[index] = data;
	return;
}

void Storage::writeStrategySum(int64_t index, double data) {
	assert(index + 1 < strategy_sum.max_size());
	if (index >= strategy_sum.size()) {
		strategy_sum.resize(index + 1, 0);
	}
	strategy_sum[index] = data;
	return;
}

long Storage::getRegretSumSize() {
	return regret_sum.size();
}

long Storage::getStrategySumSize() {
	return strategy_sum.size();
}

void Storage::resizeRegretSum(long size) {
	assert(size < regret_sum.max_size());

	regret_sum.resize(size, 0);
}

void Storage::resizeStrategySum(long size) {
	assert(size < strategy_sum.max_size());

	strategy_sum.resize(size, 0);
}