#pragma once

#include "Deck.h"
#include "DeckFormat.h"

#include <string>
#include <vector>

struct ImportedDeckEntry
{
    int quantity;
    std::string cardName;
    std::string section;
};

class DeckListIO
{
public:
    static bool exportMTGO(
        const Deck& deck,
        const std::string& filename
    );

    static std::string readTextFile(
        const std::string& filename
    );

    static std::vector<ImportedDeckEntry>
        parseMTGO(
            const std::string& deckText,
            DeckFormat format
        );
};