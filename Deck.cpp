#include "Deck.h"
#include "StringUtils.h"
#include "DeckFormatUtils.h"
#include <nlohmann/json.hpp>

Deck::Deck()
    : name(""),
    format(DeckFormat::Commander)
{
}

Deck::Deck(
    const std::string& name,
    DeckFormat format
)
    : name(name),
    format(format)
{
}

const std::string& Deck::getName() const
{
    return name;
}

DeckFormat Deck::getFormat() const
{
    return format;
}

const std::vector<DeckEntry>& Deck::getEntries() const
{
    return entries;
}

void Deck::addCard(
    const Card& card,
    int quantity,
    const std::string& section
)
{
    for (auto& entry : entries)
    {
        if (toLower(entry.getCard().getName()) == toLower(card.getName())
            && toLower(entry.getSection()) == section)
        {
            entry.setQuantity(entry.getQuantity() + quantity);
            return;
        }
    }

    entries.emplace_back(card, quantity, section);
}

bool Deck::removeCard(
    const std::string& cardName,
    int quantity
)
{
    std::string targetName = toLower(cardName);

    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        if (toLower(it->getCard().getName()) == targetName)
        {
            int currentQuantity = it->getQuantity();

            if (quantity >= currentQuantity)
            {
                entries.erase(it);
            }
            else
            {
                it->setQuantity(currentQuantity - quantity);
            }

            return true;
        }
    }

    return false;
}

bool Deck::setCardQuantity(
    const std::string& cardName,
    int quantity
)
{
    std::string targetName = toLower(cardName);

    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        if (toLower(it->getCard().getName()) == targetName)
        {
            if (quantity <= 0)
            {
                entries.erase(it);
            }
            else
            {
                it->setQuantity(quantity);
            }

            return true;
        }
    }

    return false;
}

int Deck::getTotalCardCount() const
{
    int total = 0;

    for (const auto& entry : entries)
    {
        total += entry.getQuantity();
    }

    return total;
}

int Deck::getUniqueCardCount() const
{
    return static_cast<int>(entries.size());
}

double Deck::getAverageManaValue() const
{
    int totalCards = 0;
    double totalManaValue = 0.0;

    for (const auto& entry : entries)
    {
        totalCards += entry.getQuantity();

        totalManaValue +=
            entry.getCard().getManaValue()
            * entry.getQuantity();
    }

    if (totalCards == 0)
    {
        return 0.0;
    }

    return totalManaValue / totalCards;
}

void to_json(nlohmann::json& j, const Deck& deck)
{
    j = nlohmann::json{
        {"name", deck.name},
        {"format", deckFormatToString(deck.format)},
        {"entries", deck.entries}
    };
}

void from_json(const nlohmann::json& j, Deck& deck)
{
    j.at("name").get_to(deck.name);

    std::string formatString =
        j.value("format", "Commander");

    deck.format =
        stringToDeckFormat(formatString);

    j.at("entries").get_to(deck.entries);
}