#pragma once

#include "Deck.h"
#include "DeckRepository.h"
#include "ScryfallClient.h"

#include <cstddef>
#include <string>
#include <vector>
#include <optional>

class DevotionZeroApp
{
private:
    std::vector<Deck> decks;

    std::optional<Card> selectCardFromScryfall();

    DeckRepository repository;
    ScryfallClient scryfall;

    bool running;

    void showMainMenu() const;
    void showDeckMenu(const Deck& deck) const;

    void createDeck();
    void listDecks() const;
    void openDeck();
    void deleteDeck();
    void saveDecks();

    void manageDeck(std::size_t deckIndex);

    void searchAndAddCard(Deck& deck);
    void viewDeck(const Deck& deck) const;
    void removeCardFromDeck(Deck& deck);
    void editCardQuantity(Deck& deck);
    void editCommander(Deck& deck);
    void exportDeckMTGO(const Deck& deck);
    void importMTGODeck();

    int readInt(
        const std::string& prompt,
        int minValue,
        int maxValue
    ) const;

    std::string readLine(
        const std::string& prompt
    ) const;

public:
    DevotionZeroApp();

    void run();
};