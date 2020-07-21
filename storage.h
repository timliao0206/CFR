#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>

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
	int getAverageStrategy(const int64_t index, const int bucket, const int num_action, std::vector<double>& strategy);
	void updateRegret(const int64_t index, const int bucket, std::vector<double> regret);
	void updateStrategySum(const int64_t index, const int bucket, std::vector<double> strategy);
	int sampleAction(const int64_t index, const int bucket, const int num_action);
	long getRegretSumSize();
	long getStrategySumSize();
	void resizeRegretSum(long size);
	void resizeStrategySum(long size);

	void printRegretSum(std::string fileName);
	void printStrategySum(std::string fileName);

	void writeRegretSum(int64_t index, double data);
	void writeStrategySum(int64_t index, double data);
};

#endif