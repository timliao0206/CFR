#include "FiveCardBucket.h"
#include <algorithm>

using std::vector;

vector< vector< vector< vector< vector<uint8_t> > > > > all_bucket;

bool compare_strength(const hand_cards one, const hand_cards two)
{
	return one.strength < two.strength;
}

void calculateFiveCardBucket(uint8_t num_buckets)
{
	const int tem = 52;
	std::fstream  file;
	file.open("FiveCardHandStrength.txt", std::ios::in);
	all_bucket.clear();
	all_bucket.shrink_to_fit();
	vector< vector< vector< vector< vector<uint16_t> > > > > all2(tem, vector< vector< vector< vector<uint16_t> > > >(tem, vector< vector< vector<uint16_t> > >(tem, vector< vector<uint16_t> >(tem, vector<uint16_t>(tem, 0.0)))));
	for (uint8_t h1 = 0; h1 < 52; ++h1)
	{
		for (uint8_t h2 = h1 + 1; h2 < 52; ++h2)
		{
			for (uint8_t b1 = 0; b1 < 52; ++b1)
			{
				while (b1 == h1 || b1 == h2)
					++b1;
				if (b1 > 51)
					break;
				for (uint8_t b2 = b1 + 1; b2 < 52; ++b2)
				{
					while (b2 == h1 || b2 == h2)
						++b2;
					if (b2 > 51)
						break;
					for (uint8_t b3 = b2 + 1; b3 < 52; ++b3)
					{
						while (b3 == h1 || b3 == h2)
							++b3;
						if (b3 > 51)
							break;
						file >> all2[h1][h2][b1][b2][b3];
					}
				}
			}
		}
	}
	file.close();
	// std::cout << all2[41][43][36][40][50] << std::endl;
	std::fstream  file2;
	file2.open("FiveCardBuckets.txt", std::ios::out);
	std::string for_output;
	vector<hand_cards> for_sort;
	for (uint8_t b1 = 0; b1 < 52; ++b1)
	{
		for (uint8_t b2 = b1 + 1; b2 < 52; ++b2)
		{
			for (uint8_t b3 = b2 + 1; b3 < 52; ++b3)
			{
				for_sort.clear();
				for_sort.shrink_to_fit();
				for (uint8_t h1 = 0; h1 < 52; ++h1)
				{
					while (h1 == b1 || h1 == b2 || h1 == b3)
						++h1;
					if (h1 > 51)
						break;
					for (uint8_t h2 = h1 + 1; h2 < 52; ++h2)
					{
						while (h2 == b1 || h2 == b2 || h2 == b3)
							++h2;
						if (h2 > 51)
							break;
						hand_cards now_cards;
						now_cards.h1 = h1;
						now_cards.h2 = h2;
						now_cards.strength = all2[h1][h2][b1][b2][b3];
						for_sort.push_back(now_cards);
					}
				}
				sort(for_sort.begin(), for_sort.end(), compare_strength);
				vector< vector<uint8_t> > bucket_drawer(tem, vector <uint8_t>(tem, _UI8_MAX));
				uint8_t cur_bucket = 0;
				int counter = 0;
				uint8_t max_bucket = num_buckets;
				int bucket_size = for_sort.size() / max_bucket;
				for (uint32_t i = 0; i < for_sort.size(); ++i)
				{
					bucket_drawer[(uint16_t)for_sort[i].h1][(uint16_t)for_sort[i].h2] = cur_bucket;
					counter++;
					if (counter > bucket_size)
					{
						counter = 0;
						cur_bucket++;
					}
				}
				for (uint8_t h1 = 0; h1 < 52; ++h1)
				{
					while (h1 == b1 || h1 == b2 || h1 == b3)
						++h1;
					if (h1 > 51)
						break;
					for (uint8_t h2 = h1 + 1; h2 < 52; ++h2)
					{
						while (h2 == b1 || h2 == b2 || h2 == b3)
							++h2;
						if (h2 > 51)
							break;
						file2 << (int)bucket_drawer[h1][h2] << " ";
					}
				}
			}
		}
	}
	file2.close();
	all2.clear();
	all2.shrink_to_fit();
	all_bucket.assign(tem, vector< vector< vector< vector<uint8_t> > > >(tem, vector< vector< vector<uint8_t> > >(tem, vector< vector<uint8_t> >(tem, vector<uint8_t>(tem, UINT8_MAX)))));
	file2.open("FiveCardBuckets.txt", std::ios::in);
	uint16_t uint8_t_buffer;
	for (uint8_t b1 = 0; b1 < 52; ++b1)
	{
		for (uint8_t b2 = b1 + 1; b2 < 52; ++b2)
		{
			for (uint8_t b3 = b2 + 1; b3 < 52; ++b3)
			{
				for (uint8_t h1 = 0; h1 < 52; ++h1)
				{
					while (h1 == b1 || h1 == b2 || h1 == b3)
						++h1;
					if (h1 > 51)
						break;
					for (uint8_t h2 = h1 + 1; h2 < 52; ++h2)
					{
						while (h2 == b1 || h2 == b2 || h2 == b3)
							++h2;
						if (h2 > 51)
							break;
						file2 >> uint8_t_buffer;
						all_bucket[h1][h2][b1][b2][b3] = (uint8_t)uint8_t_buffer;
					}
				}
			}
		}
	}
	file2.close();
}

uint8_t getFlopBucketByHandStrength(uint8_t first_hand_card, uint8_t second_hand_card, uint8_t first_board_card, uint8_t second_board_card, uint8_t third_board_card)
{
	return all_bucket[first_hand_card][second_hand_card][first_board_card][second_board_card][third_board_card];
}

uint8_t getFlopBucketByHandStrength(const int8_t board_cards[7/*max board cards*/],const int8_t hole_cards[10/*max player*/][3/*max hole cards*/],const int position)
{
	uint8_t for_sort[3];
	for_sort[0] = board_cards[0];
	for_sort[1] = board_cards[1];
	for_sort[2] = board_cards[2];
	std::sort(for_sort, for_sort + 3);
	return getFlopBucketByHandStrength((uint8_t)std::min(hole_cards[position][0], hole_cards[position][1]), 
		(uint8_t)std::max(hole_cards[position][0], hole_cards[position][1]), for_sort[0], for_sort[1], for_sort[2]);
}