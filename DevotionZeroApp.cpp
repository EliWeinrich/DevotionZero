#include "DevotionZeroApp.h"
#include "DeckAnalyzer.h"
#include "DeckFormat.h"
#include "DeckValidator.h"
#include "CommanderRules.h"
#include "DeckOrganizer.h"

#include "StringUtils.h"
#include "DeckFormatUtils.h"
#include "DeckListIO.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include <chrono>
#include <thread>


DevotionZeroApp::DevotionZeroApp()
    : repository("decks.json"),
    running(true)
{
}


void DevotionZeroApp::run()
{
    decks = repository.loadDecks();

    std::cout
        << "Loaded "
        << decks.size()
        << " saved deck(s).\n";

    while (running)
    {
        showMainMenu();

        int choice = readInt(
            "Choose an option: ",
            1,
            7
        );

        switch (choice)
        {
        case 1:
            createDeck();
            break;

        case 2:
            importMTGODeck();
            break;

        case 3:
            listDecks();
            break;

        case 4:
            openDeck();
            break;

        case 5:
            deleteDeck();
            break;

        case 6:
            saveDecks();
            break;

        case 7:
            saveDecks();
            running = false;
            std::cout << "\nExiting DevotionZero.\n";
            break;
        }
    }
}


void DevotionZeroApp::showMainMenu() const
{
    std::cout << "\n===== Devotion Zero =====\n";
    std::cout << "1. Create Deck\n";
    std::cout << "2. Import Deck (MTGO)\n";
    std::cout << "3. List Decks\n";
    std::cout << "4. Open Deck\n";
    std::cout << "5. Delete Deck\n";
    std::cout << "6. Save\n";
    std::cout << "7. Exit\n";
}


void DevotionZeroApp::showDeckMenu(
    const Deck& deck
) const
{
    std::cout
        << "\n===== "
        << deck.getName()
        << " =====\n";

    std::cout << "1. Search and Add Card\n";
    std::cout << "2. View Deck\n";
    std::cout << "3. Remove Card\n";
    std::cout << "4. Set Card Quantity\n";
    std::cout << "5. Edit Commanders\n";
    std::cout << "6. View Statistics\n";
    std::cout << "7. Save\n";
    std::cout << "8. Export MTGO\n";
    std::cout << "9. Back\n";
}


void DevotionZeroApp::createDeck()
{
    std::string name =
        readLine("Enter deck name: ");

    if (name.empty())
    {
        std::cout
            << "Deck name cannot be empty.\n";
        return;
    }

    std::cout << "\nChoose a format:\n";
    std::cout << "1. Commander\n";
    std::cout << "2. Modern\n";
    std::cout << "3. Standard\n";
    std::cout << "4. Pauper\n";

    int formatChoice =
        readInt(
            "Choose an option: ",
            1,
            4
        );

    DeckFormat format;

    switch (formatChoice)
    {
    case 1:
        format = DeckFormat::Commander;
        break;

    case 2:
        format = DeckFormat::Modern;
        break;

    case 3:
        format = DeckFormat::Standard;
        break;

    case 4:
        format = DeckFormat::Pauper;
        break;
    }

    decks.emplace_back(
        name,
        format
    );

    repository.saveDecks(decks);

    std::cout
        << "Deck created successfully.\n";
}

void DevotionZeroApp::importMTGODeck()
{
    std::cout
        << "\n===== Import MTGO Deck =====\n";

    std::cout
        << "1. Paste decklist\n";

    std::cout
        << "2. Import from .txt file\n";

    std::cout
        << "3. Cancel\n";

    int importChoice =
        readInt(
            "Choose an option: ",
            1,
            3
        );

    if (importChoice == 3)
    {
        return;
    }

    std::string deckText;

    if (importChoice == 1)
    {
        std::cout
            << "\nPaste your MTGO decklist below.\n";

        std::cout
            << "Enter END on its own line when finished.\n\n";

        std::string line;

        while (true)
        {
            std::getline(
                std::cin,
                line
            );

            if (line == "END")
            {
                break;
            }

            deckText +=
                line + "\n";
        }
    }
    else
    {
        std::string filename =
            readLine(
                "Enter path to .txt file: "
            );

        deckText =
            DeckListIO::readTextFile(
                filename
            );

        if (deckText.empty())
        {
            std::cout
                << "Could not read the file.\n";

            return;
        }
    }

    std::string deckName =
        readLine(
            "Enter name for imported deck: "
        );

    if (deckName.empty())
    {
        std::cout
            << "Deck name cannot be empty.\n";

        return;
    }

    std::cout << "\nChoose deck format:\n";
    std::cout << "1. Commander\n";
    std::cout << "2. Modern\n";
    std::cout << "3. Standard\n";
    std::cout << "4. Pauper\n";

    int formatChoice =
        readInt(
            "Choose an option: ",
            1,
            4
        );

    DeckFormat format;

    switch (formatChoice)
    {
    case 1:
        format =
            DeckFormat::Commander;
        break;

    case 2:
        format =
            DeckFormat::Modern;
        break;

    case 3:
        format =
            DeckFormat::Standard;
        break;

    case 4:
        format =
            DeckFormat::Pauper;
        break;
    }

    std::vector<ImportedDeckEntry>
        importedEntries =
        DeckListIO::parseMTGO(
            deckText,
            format
        );

    if (importedEntries.empty())
    {
        std::cout
            << "No valid cards were found in the decklist.\n";

        return;
    }

    Deck importedDeck(
        deckName,
        format
    );

    std::cout
        << "\nImporting "
        << importedEntries.size()
        << " deck entries...\n\n";

    // Build a list of names for Scryfall.
    std::vector<std::string> cardNames;
    std::unordered_set<std::string> seenNames;

    cardNames.reserve(
        importedEntries.size()
    );

    for (const auto& importedEntry : importedEntries)
    {
        std::string normalizedName =
            toLower(importedEntry.cardName);

        if (seenNames.insert(normalizedName).second)
        {
            cardNames.push_back(
                importedEntry.cardName
            );
        }
    }

    // Fetch all card data using Scryfall's
    // collection endpoint in batches.
    std::vector<Card> fetchedCards;

    try
    {
        std::cout
            << "Fetching card data from Scryfall...\n";

        fetchedCards =
            scryfall.fetchCardsByNames(
                cardNames
            );
    }
    catch (const ScryfallRateLimitException& e)
    {
        std::cout
            << "\nImport cancelled: "
            << e.what()
            << "\n";

        std::cout
            << "The deck was not saved.\n";

        return;
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nImport failed: "
            << e.what()
            << "\n";

        return;
    }

    // Make the fetched cards easy to look up
    // by name.
    std::unordered_map<std::string, Card>
        cardsByName;

    for (const Card& card : fetchedCards)
    {
        cardsByName.insert_or_assign(
            toLower(card.getName()),
            card
        );
    }

    int successfulEntries = 0;
    int failedEntries = 0;


    // Rebuild the deck using the imported
    // quantities and sections.
    for (const auto& importedEntry : importedEntries)
    {
        std::string lookupName =
            toLower(
                importedEntry.cardName
            );

        auto it =
            cardsByName.find(
                lookupName
            );

        if (it == cardsByName.end())
        {
            try
            {
                std::cout
                    << "Trying alternate lookup: "
                    << importedEntry.cardName
                    << "\n";

                Card fallbackCard =
                    scryfall.fetchCardByFuzzyName(
                        importedEntry.cardName
                    );

                importedDeck.addCard(
                    fallbackCard,
                    importedEntry.quantity,
                    importedEntry.section
                );

                ++successfulEntries;

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(150)
                );

                continue;
            }
            catch (const ScryfallRateLimitException& e)
            {
                std::cout
                    << "\nFallback lookup stopped: "
                    << e.what()
                    << "\n";

                std::cout
                    << "No further fallback requests "
                    << "will be made.\n";

                return;
            }
            catch (const std::exception&)
            {
                ++failedEntries;

                std::cout
                    << "Could not resolve: "
                    << importedEntry.cardName
                    << "\n";

                continue;
            }
        }

        importedDeck.addCard(
            it->second,
            importedEntry.quantity,
            importedEntry.section
        );

        ++successfulEntries;
    }

    if (successfulEntries == 0)
    {
        std::cout
            << "\nImport failed. No cards were successfully loaded.\n";

        return;
    }

    decks.push_back(
        importedDeck
    );

    repository.saveDecks(
        decks
    );

    std::cout
        << "\n===== Import Complete =====\n";

    std::cout
        << "Imported entries: "
        << successfulEntries
        << "\n";

    std::cout
        << "Failed entries: "
        << failedEntries
        << "\n";

    std::cout
        << "Total cards: "
        << importedDeck.getTotalCardCount()
        << "\n";

    std::cout
        << "\n===== Deck Validation =====\n";

    std::vector<ValidationIssue> issues =
        DeckValidator::validate(
            importedDeck
        );

    if (issues.empty())
    {
        std::cout
            << "No structural issues found.\n";
    }
    else
    {
        for (const auto& issue : issues)
        {
            if (issue.isError)
            {
                std::cout << "ERROR: ";
            }
            else
            {
                std::cout << "WARNING: ";
            }

            std::cout
                << issue.message
                << "\n";
        }
    }
}


void DevotionZeroApp::listDecks() const
{
    if (decks.empty())
    {
        std::cout
            << "\nNo saved decks found.\n";
        return;
    }

    std::cout
        << "\n===== Saved Decks =====\n";

    for (std::size_t i = 0;
        i < decks.size();
        ++i)
    {
        std::cout
            << (i + 1)
            << ". "
            << decks[i].getName()
            << " ["
            << deckFormatToString(decks[i].getFormat())
            << "]"
            << " - "
            << decks[i].getTotalCardCount()
            << " cards\n";
    }
}


void DevotionZeroApp::openDeck()
{
    if (decks.empty())
    {
        std::cout
            << "\nNo decks available to open.\n";
        return;
    }

    listDecks();

    int choice = readInt(
        "\nChoose a deck: ",
        1,
        static_cast<int>(decks.size())
    );

    std::size_t deckIndex =
        static_cast<std::size_t>(
            choice - 1
            );

    manageDeck(deckIndex);
}


void DevotionZeroApp::deleteDeck()
{
    if (decks.empty())
    {
        std::cout
            << "\nNo decks available to delete.\n";
        return;
    }

    listDecks();

    int choice = readInt(
        "\nChoose a deck to delete: ",
        1,
        static_cast<int>(decks.size())
    );

    std::size_t index =
        static_cast<std::size_t>(
            choice - 1
            );

    std::cout
        << "Deleting deck: "
        << decks[index].getName()
        << "\n";

    decks.erase(
        decks.begin() + index
    );

    repository.saveDecks(decks);

    std::cout
        << "Deck deleted.\n";
}


void DevotionZeroApp::saveDecks()
{
    repository.saveDecks(decks);

    std::cout
        << "Decks saved successfully.\n";
}


void DevotionZeroApp::manageDeck(
    std::size_t deckIndex
)
{
    bool managingDeck = true;

    while (managingDeck)
    {
        Deck& deck =
            decks[deckIndex];

        showDeckMenu(deck);

        int choice = readInt(
            "Choose an option: ",
            1,
            9
        );

        switch (choice)
        {
        case 1:
            searchAndAddCard(deck);
            break;

        case 2:
            viewDeck(deck);
            break;

        case 3:
            removeCardFromDeck(deck);
            break;

        case 4:
            editCardQuantity(deck);
            break;

        case 5:
            editCommander(deck);
            break;

        case 6:
        {
            std::cout << "\n===== Deck Statistics =====\n";

            std::cout << "Total Cards: "
                << deck.getTotalCardCount()
                << "\n";

            std::cout << "Unique Cards: "
                << deck.getUniqueCardCount()
                << "\n";

            std::cout << "Lands: "
                << DeckAnalyzer::getLandCount(deck)
                << "\n";

            std::cout << "Nonlands: "
                << DeckAnalyzer::getNonlandCount(deck)
                << "\n";

            std::cout << "Average Mana Value: "
                << DeckAnalyzer::getAverageManaValue(deck)
                << "\n";

            std::cout << "Average Mana Value (Nonlands): "
                << DeckAnalyzer::getAverageNonlandManaValue(deck)
                << "\n";

            std::cout << "\n===== Deck Validation =====\n";

            std::vector<ValidationIssue> issues =
                DeckValidator::validate(deck);

            if (issues.empty())
            {
                std::cout
                    << "No structural issues found.\n";
            }
            else
            {
                for (const auto& issue : issues)
                {
                    if (issue.isError)
                    {
                        std::cout << "ERROR: ";
                    }
                    else
                    {
                        std::cout << "WARNING: ";
                    }

                    std::cout
                        << issue.message
                        << "\n";
                }
            }

            break;
        }

        case 7:
            repository.saveDecks(decks);

            std::cout
                << "Deck saved.\n";
            break;

        case 8:
            exportDeckMTGO(deck);
            break;

        case 9:
            repository.saveDecks(decks);
            managingDeck = false;
            break;
        }
    }
}


void DevotionZeroApp::searchAndAddCard(
    Deck& deck
)
{
    std::optional<Card> selectedCard =
        selectCardFromScryfall();

    if (!selectedCard.has_value())
    {
        return;
    }

    Card card =
        selectedCard.value();

    std::string section;

    if (deck.getFormat() == DeckFormat::Commander)
    {
        std::cout << "\nChoose section:\n";
        std::cout << "1. Mainboard\n";
        std::cout << "2. Sideboard\n";
        std::cout << "3. Commander\n";

        int sectionChoice =
            readInt(
                "Choose an option: ",
                1,
                3
            );

        switch (sectionChoice)
        {
        case 1:
            section = "Mainboard";
            break;

        case 2:
            section = "Sideboard";
            break;

        case 3:
            if (!CommanderRules::canBeCommander(card))
            {
                std::cout
                    << card.getName()
                    << " cannot be used as a commander.\n";

                return;
            }

            section = "Commander";
            break;
        }
    }
    else
    {
        std::cout << "\nChoose section:\n";
        std::cout << "1. Mainboard\n";
        std::cout << "2. Sideboard\n";

        int sectionChoice =
            readInt(
                "Choose an option: ",
                1,
                2
            );

        switch (sectionChoice)
        {
        case 1:
            section = "Mainboard";
            break;

        case 2:
            section = "Sideboard";
            break;
        }
    }

    int quantity;

    if (section == "Commander")
    {
        quantity = 1;

        std::cout
            << "Commander quantity set to 1.\n";
    }
    else
    {
        quantity =
            readInt(
                "Enter quantity: ",
                1,
                100
            );
    }

    deck.addCard(
        card,
        quantity,
        section
    );

    repository.saveDecks(decks);

    std::cout
        << "Card added successfully.\n";

    std::cout
        << "Deck now contains "
        << deck.getTotalCardCount()
        << " total cards and "
        << deck.getUniqueCardCount()
        << " unique cards.\n";
}

void DevotionZeroApp::viewDeck(
    const Deck& deck
) const
{
    std::cout
        << "\n===== "
        << deck.getName()
        << " =====\n";

    std::cout
        << "Format: "
        << deckFormatToString(deck.getFormat())
        << "\n";

    if (deck.getEntries().empty())
    {
        std::cout
            << "\nThis deck is empty.\n";
        return;
    }

    std::cout
        << "\n===== Deck List =====\n";

    auto groups =
        DeckOrganizer::groupEntries(deck);

    const std::vector<std::string> groupOrder = {
        "Commanders",
        "Creatures",
        "Planeswalkers",
        "Artifacts",
        "Enchantments",
        "Sorceries",
        "Instants",
        "Battles",
        "Lands",
        "Other",
        "Sideboard"
    };

    for (const std::string& groupName : groupOrder)
    {
        auto it = groups.find(groupName);

        if (it == groups.end()
            || it->second.empty())
        {
            continue;
        }

        std::cout
            << "\n===== "
            << groupName
            << " =====\n";

        for (const auto& entry : it->second)
        {
            std::cout
                << entry.getQuantity()
                << "x "
                << entry.getCard().getName()
                << " | CMC "
                << entry.getCard().getManaValue()
                << "\n";
        }
    }

    std::cout
        << "\nTotal Cards: "
        << deck.getTotalCardCount()
        << "\n";

    std::cout
        << "Unique Cards: "
        << deck.getUniqueCardCount()
        << "\n";

    std::cout
        << "Average Mana Value (CMC): "
        << deck.getAverageManaValue()
        << "\n";
}


void DevotionZeroApp::removeCardFromDeck(Deck& deck)
{
    if (deck.getEntries().empty())
    {
        std::cout << "\nThis deck has no cards to remove.\n";
        return;
    }

    viewDeck(deck);

    std::string cardName =
        readLine("\nEnter the card name to remove: ");

    int quantity =
        readInt(
            "Enter quantity to remove: ",
            1,
            100
        );

    if (deck.removeCard(cardName, quantity))
    {
        repository.saveDecks(decks);

        std::cout
            << "Card quantity updated successfully.\n";
    }
    else
    {
        std::cout
            << "Card not found in this deck.\n";
    }
}

void DevotionZeroApp::editCommander(
    Deck& deck
)
{
    if (deck.getFormat() != DeckFormat::Commander)
    {
        std::cout
            << "\nThis deck does not use commanders.\n";

        return;
    }

    std::vector<Card> commanders;

    for (const auto& entry : deck.getEntries())
    {
        if (toLower(entry.getSection()) == "commander")
        {
            commanders.push_back(
                entry.getCard()
            );
        }
    }

    std::cout
        << "\n===== Commander Management =====\n";

    if (commanders.empty())
    {
        std::cout
            << "No commander currently selected.\n";
    }
    else
    {
        for (std::size_t i = 0;
            i < commanders.size();
            ++i)
        {
            std::cout
                << (i + 1)
                << ". "
                << commanders[i].getName()
                << "\n";
        }
    }

    std::cout << "\n1. Replace Commander\n";
    std::cout << "2. Add Second Commander\n";
    std::cout << "3. Remove Commander\n";
    std::cout << "4. Back\n";

    int choice =
        readInt(
            "Choose an option: ",
            1,
            4
        );

    if (choice == 4)
    {
        return;
    }

    if (choice == 1)
    {
        std::optional<Card> selectedCard =
            selectCardFromScryfall();

        if (!selectedCard.has_value())
        {
            return;
        }

        Card card =
            selectedCard.value();

        if (!CommanderRules::canBeCommander(card))
        {
            std::cout
                << card.getName()
                << " cannot be used as a commander.\n";

            return;
        }

        // Remove existing commanders
        for (const Card& name : commanders)
        {
            deck.removeCard(name.getName(), 1);
        }

        deck.addCard(
            card,
            1,
            "Commander"
        );

        repository.saveDecks(decks);

        std::cout
            << "Commander replaced successfully.\n";

        return;
    }

    if (choice == 2)
    {
        if (commanders.empty())
        {
            std::cout
                << "Add a primary commander first.\n";

            return;
        }

        if (commanders.size() >= 2)
        {
            std::cout
                << "This deck already has two commanders.\n";

            return;
        }

        std::optional<Card> selectedCard =
            selectCardFromScryfall();

        if (!selectedCard.has_value())
        {
            return;
        }

        Card newCommander =
            selectedCard.value();

        const Card& existingCommander =
            commanders[0];

        if (!CommanderRules::canPartnerTogether(
            existingCommander,
            newCommander
        ))
        {
            std::cout
                << existingCommander.getName()
                << " and "
                << newCommander.getName()
                << " cannot legally be commanders together.\n";

            return;
        }

        deck.addCard(
            newCommander,
            1,
            "Commander"
        );

        repository.saveDecks(decks);

        std::cout
            << "Second commander added successfully.\n";

        return;
    }

    if (choice == 3)
    {
        if (commanders.empty())
        {
            std::cout
                << "There is no commander to remove.\n";

            return;
        }

        if (commanders.size() == 1)
        {
            deck.removeCard(
                commanders[0].getName(),
                1
            );
        }
        else
        {
            std::cout
                << "\nChoose commander to remove:\n";

            for (std::size_t i = 0;
                i < commanders.size();
                ++i)
            {
                std::cout
                    << (i + 1)
                    << ". "
                    << commanders[i].getName()
                    << "\n";
            }

            int removeChoice =
                readInt(
                    "Choose an option: ",
                    1,
                    static_cast<int>(
                        commanders.size()
                        )
                );

            deck.removeCard(
                commanders[
                    removeChoice - 1
                ].getName(),
                1
            );
        }

        repository.saveDecks(decks);

        std::cout
            << "Commander removed successfully.\n";
    }
}

void DevotionZeroApp::editCardQuantity(
    Deck& deck
)
{
    if (deck.getEntries().empty())
    {
        std::cout << "\nThis deck is empty.\n";
        return;
    }

    viewDeck(deck);

    std::string cardName =
        readLine(
            "\nEnter the card name to edit: "
        );

    int quantity =
        readInt(
            "Enter new quantity: ",
            0,
            100
        );

    if (deck.setCardQuantity(cardName, quantity))
    {
        repository.saveDecks(decks);

        std::cout
            << "Card quantity updated successfully.\n";
    }
    else
    {
        std::cout
            << "Card not found in this deck.\n";
    }
}


int DevotionZeroApp::readInt(
    const std::string& prompt,
    int minValue,
    int maxValue
) const
{
    int value;

    while (true)
    {
        std::cout << prompt;

        std::cin >> value;

        if (!std::cin.fail() &&
            value >= minValue &&
            value <= maxValue)
        {
            std::cin.ignore(
                std::numeric_limits<
                std::streamsize
                >::max(),
                '\n'
            );

            return value;
        }

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<
            std::streamsize
            >::max(),
            '\n'
        );

        std::cout
            << "Invalid input. Enter a number between "
            << minValue
            << " and "
            << maxValue
            << ".\n";
    }
}


std::string DevotionZeroApp::readLine(
    const std::string& prompt
) const
{
    std::string input;

    std::cout << prompt;

    std::getline(
        std::cin,
        input
    );

    return input;
}

std::optional<Card> DevotionZeroApp::selectCardFromScryfall()
{
    std::string query =
        readLine(
            "Enter card name: "
        );

    if (query.empty())
    {
        std::cout << "Search cancelled.\n";
        return std::nullopt;
    }

    try
    {
        std::vector<std::string> matches =
            scryfall.autocompleteCardNames(query);

        if (matches.empty())
        {
            std::cout << "No matching cards found.\n";
            return std::nullopt;
        }

        std::string selectedName;

        if (matches.size() == 1)
        {
            selectedName = matches[0];
        }
        else
        {
            std::cout << "\nMatching cards:\n";

            for (std::size_t i = 0; i < matches.size(); ++i)
            {
                std::cout
                    << (i + 1)
                    << ". "
                    << matches[i]
                    << "\n";
            }

            int choice =
                readInt(
                    "Choose a card: ",
                    1,
                    static_cast<int>(matches.size())
                );

            selectedName =
                matches[choice - 1];
        }

        Card card =
            scryfall.fetchCardByName(selectedName);

        std::cout << "\n===== Card Selected =====\n";

        std::cout
            << "Name: "
            << card.getName()
            << "\n";

        std::cout
            << "Mana Cost: "
            << card.getManaCost()
            << "\n";

        std::cout
            << "CMC: "
            << card.getManaValue()
            << "\n";

        std::cout
            << "Type: "
            << card.getTypeLine()
            << "\n";

        std::cout
            << "Color Identity: "
            << card.getColorIdentity()
            << "\n";

        std::cout
            << "Oracle Text: "
            << card.getOracleText()
            << "\n";

        std::string confirm =
            readLine(
                "\nUse this card? (y/n): "
            );

        if (toLower(confirm) != "y"
            && toLower(confirm) != "yes")
        {
            std::cout << "Card selection cancelled.\n";
            return std::nullopt;
        }

        return card;
    }
    catch (const std::exception& e)
    {
        std::cout
            << "Card search failed: "
            << e.what()
            << "\n";

        return std::nullopt;
    }
}

void DevotionZeroApp::exportDeckMTGO(
    const Deck& deck
)
{
    std::string folderPath =
        readLine(
            "Enter export folder path: "
        );

    if (folderPath.empty())
    {
        std::cout
            << "Export cancelled.\n";
        return;
    }

    std::string filename =
        readLine(
            "Enter export filename: "
        );

    if (filename.empty())
    {
        std::cout
            << "Export cancelled.\n";
        return;
    }

    if (filename.size() < 4 ||
        filename.substr(filename.size() - 4) != ".txt")
    {
        filename += ".txt";
    }

    std::filesystem::path fullPath =
        std::filesystem::path(folderPath)
        / filename;

    if (DeckListIO::exportMTGO(
        deck,
        fullPath.string()
    ))
    {
        std::cout
            << "Deck exported successfully to:\n"
            << fullPath.string()
            << "\n";
    }
    else
    {
        std::cout
            << "Failed to export deck.\n";
    }
}