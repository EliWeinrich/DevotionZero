// DevotionZero.cpp 
// Elijah Weinrich
// Started 9/24/2026

#include "DevotionZeroApp.h"
#include "iostream"

int main()
{
    DevotionZeroApp app;

    app.run();

    return 0;
}

void displayDeck(const Deck& deck)
{
    std::cout << "\n===== "
        << deck.getName()
        << " =====\n";

    for (const auto& entry : deck.getEntries())
    {
        std::cout
            << entry.getQuantity()
            << "x "
            << entry.getCard().getName()
            << "\n";
    }

    std::cout << "\nTotal Cards: "
        << deck.getTotalCardCount()
        << "\n";

    std::cout << "Unique Cards: "
        << deck.getUniqueCardCount()
        << "\n";

    std::cout << "Average Mana Value: "
        << deck.getAverageManaValue()
        << "\n";
}

