#pragma once
#ifndef EVALHANDTABLE_H
#define EVALHANDTABLE_H

#include <inttypes.h>
#include "const.h"

typedef union {
	uint16_t bySuit[4];
	uint64_t cards;
}CardSet;

struct eval {
private:

	static const int twoCardRank[52][52];
	static const uint16_t oneSuitVal[8192];
	static const uint16_t pairOtherVal[8192];
	static const uint16_t anySuitVal[8192];
	static const uint8_t topBit[8192];
	static const uint8_t tripsOtherVal[8192];
	static const uint16_t quadsVal[13];
	static const uint16_t tripsVal[13];
	static const uint16_t fullHouseOtherVal;
	static const uint16_t pairsVal[13];
	static const uint16_t twoPairOtherVal[13];

public:
	static int rankCardset(const CardSet cards);
	static CardSet emptyCardset();
	static void addCardToCardSet(CardSet* c, int suit, int rank);
	static int rankTwoCards(const CardSet cards);
	static int rankTwoCards(const int card1, const int card2);

};
#endif