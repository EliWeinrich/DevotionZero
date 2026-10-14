#pragma once

#include "DeckFormat.h"

#include <string>
#include <unordered_set>

class BanListRepository
{
public:
    static const std::unordered_set<std::string>&
        getBannedCards(DeckFormat format);

    static bool isBanned(
        const std::string& cardName,
        DeckFormat format
    );
};