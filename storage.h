#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <vector>

class Storage {
private:

	std::vector<double> regret_sum;
	std::vector<double> strategy_sum;

	const size_t num_entries_per_bucket;

	int64_t getLocalIndex(const int64_t index, const int bucket) const;

public:

	Storage(size_t per_bucket);
	~Storage();

	int getStrategy(const int64_t index, const int bucket, const int num_action, std::vector<double>& strategy);
	void updateRegret(const int64_t index, const int bucket, std::vector<double> regret);
	int sampleAction(const int64_t index, const int bucket, const int num_action);
};

#endif