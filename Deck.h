#pragma once

#include "DeckEntry.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "DeckFormat.h"

class Deck
{
private:
    std::string name;
    DeckFormat format;
    std::vector<DeckEntry> entries;

public:
    Deck();

    Deck(
        const std::string& name,
        DeckFormat format
    );

    const std::string& getName() const;
    DeckFormat getFormat() const;
    const std::vector<DeckEntry>& getEntries() const;

    void addCard(
        const Card& card,
        int quantity = 1,
        const std::string& section = "Mainboard"
    );

    bool removeCard(const std::string& cardName, int quantity);

    bool setCardQuantity(
        const std::string& cardName,
        int quantity
    );

    int getTotalCardCount() const;
    int getUniqueCardCount() const;
    double getAverageManaValue() const;

    friend void to_json(nlohmann::json& j, const Deck& deck);
    friend void from_json(const nlohmann::json& j, Deck& deck);
};