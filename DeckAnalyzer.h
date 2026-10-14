#pragma once

#include "Deck.h"

class DeckAnalyzer
{
public:
    static int getLandCount(const Deck& deck);

    static int getNonlandCount(const Deck& deck);

    static double getAverageManaValue(
        const Deck& deck
    );

    static double getAverageNonlandManaValue(
        const Deck& deck
    );
};
