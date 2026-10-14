#pragma once

#include "Deck.h"

#include <map>
#include <string>
#include <vector>

class DeckOrganizer
{
public:
    static std::string getGroupName(
        const DeckEntry& entry
    );

    static std::map<
        std::string,
        std::vector<DeckEntry>
    > groupEntries(
        const Deck& deck
    );
};