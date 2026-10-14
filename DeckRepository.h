#pragma once

#include "Deck.h"

#include <string>
#include <vector>

class DeckRepository
{
private:
    std::string filename;

public:
    DeckRepository(const std::string& filename);

    std::vector<Deck> loadDecks() const;
    void saveDecks(const std::vector<Deck>& decks) const;
};