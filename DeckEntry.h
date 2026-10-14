#pragma once

#include "Card.h"
#include <string>
#include <nlohmann/json.hpp>

class DeckEntry
{
private:
    Card card;
    int quantity;
    std::string section;

public:
    DeckEntry();

    DeckEntry(
        const Card& card,
        int quantity,
        const std::string& section = "Mainboard"
    );

    const Card& getCard() const;
    int getQuantity() const;
    const std::string& getSection() const;

    void setQuantity(int quantity);

    friend void to_json(nlohmann::json& j, const DeckEntry& entry);
    friend void from_json(const nlohmann::json& j, DeckEntry& entry);
};