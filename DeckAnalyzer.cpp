#include "DeckAnalyzer.h"

#include <string>

int DeckAnalyzer::getLandCount(
    const Deck& deck
)
{
    int landCount = 0;

    for (const auto& entry : deck.getEntries())
    {
        const Card& card = entry.getCard();

        if (
            card.getTypeLine().find("Land")
            != std::string::npos
            )
        {
            landCount += entry.getQuantity();
        }
    }

    return landCount;
}

int DeckAnalyzer::getNonlandCount(
    const Deck& deck
)
{
    return deck.getTotalCardCount()
        - getLandCount(deck);
}

double DeckAnalyzer::getAverageManaValue(
    const Deck& deck
)
{
    int totalCards = 0;
    double totalManaValue = 0.0;

    for (const auto& entry : deck.getEntries())
    {
        int quantity = entry.getQuantity();

        totalCards += quantity;

        totalManaValue +=
            entry.getCard().getManaValue()
            * quantity;
    }

    if (totalCards == 0)
    {
        return 0.0;
    }

    return totalManaValue / totalCards;
}

double DeckAnalyzer::getAverageNonlandManaValue(
    const Deck& deck
)
{
    int nonlandCards = 0;
    double totalManaValue = 0.0;

    for (const auto& entry : deck.getEntries())
    {
        const Card& card = entry.getCard();

        bool isLand =
            card.getTypeLine().find("Land")
            != std::string::npos;

        if (!isLand)
        {
            int quantity = entry.getQuantity();

            nonlandCards += quantity;

            totalManaValue +=
                card.getManaValue()
                * quantity;
        }
    }

    if (nonlandCards == 0)
    {
        return 0.0;
    }

    return totalManaValue / nonlandCards;
}