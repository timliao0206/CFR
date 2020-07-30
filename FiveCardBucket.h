#ifndef FIVE_CARD_BUCKET_H
#define FIVE_CARD_BUCKET_H

#include <bits/stdc++.h>

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

uint8_t getFlopBucketByHandStrength(const uint8_t[7/*max board cards*/], uint8_t[10/*max player*/][3/*max hole cards*/], int, uint8_t);

#endif