#include "DeckRepository.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

DeckRepository::DeckRepository(const std::string& filename)
    : filename(filename)
{
}

void DeckRepository::saveDecks(
    const std::vector<Deck>& decks
) const
{
    std::ofstream outFile(filename);

    if (!outFile)
    {
        std::cout << "Could not open "
            << filename
            << " for writing.\n";
        return;
    }

    nlohmann::json j = decks;

    outFile << j.dump(4);
}

std::vector<Deck> DeckRepository::loadDecks() const
{
    std::ifstream inFile(filename);

    if (!inFile)
    {
        return {};
    }

    try
    {
        nlohmann::json j;

        inFile >> j;

        return j.get<std::vector<Deck>>();
    }
    catch (const std::exception& e)
    {
        std::cout << "Failed to load decks: "
            << e.what()
            << "\n";

        return {};
    }
}