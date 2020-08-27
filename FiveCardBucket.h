#ifndef FIVE_CARD_BUCKET_H
#define FIVE_CARD_BUCKET_H

#include "const.h"

#include <iostream>
#include <vector>
#include <fstream>


using std::vector;

extern uint8_t cur_num_bucket;

extern vector< vector< vector< vector< vector<uint8_t> > > > > all_bucket;

struct hand_cards
{
	uint8_t h1;
	uint8_t h2;
	uint16_t strength;
};

inline bool exist_file(const std::string&);

bool compare_strength(const hand_cards, const hand_cards);

void calculateFiveCardBucket(uint8_t);

uint8_t getFlopBucketByHandStrength(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

uint8_t getFlopBucketByHandStrength(const int8_t[7],const int8_t[10][3],const int, uint8_t);

uint8_t getAllPossibleFlopBucketByHandStrength(const uint8_t[5/*all board cards*/], vector<uint8_t>&/*output*/);

#endif