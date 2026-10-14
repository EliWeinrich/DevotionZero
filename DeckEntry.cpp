#include "DeckEntry.h"
#include <nlohmann/json.hpp>

DeckEntry::DeckEntry()
    : card(),
    quantity(0),
    section("Mainboard")
{
}

DeckEntry::DeckEntry(
    const Card& card,
    int quantity,
    const std::string& section
)
    : card(card),
    quantity(quantity),
    section(section)
{
}

const Card& DeckEntry::getCard() const
{
    return card;
}

int DeckEntry::getQuantity() const
{
    return quantity;
}

const std::string& DeckEntry::getSection() const
{
    return section;
}

void DeckEntry::setQuantity(int quantity)
{
    this->quantity = quantity;
}

void to_json(nlohmann::json& j, const DeckEntry& entry)
{
    j = nlohmann::json{
        {"card", entry.card},
        {"quantity", entry.quantity},
        {"section", entry.section}
    };
}

void from_json(const nlohmann::json& j, DeckEntry& entry)
{
    j.at("card").get_to(entry.card);
    j.at("quantity").get_to(entry.quantity);
    j.at("section").get_to(entry.section);
}