#include "DeckValidator.h"

#include "BanListRepository.h"
#include "FormatRules.h"
#include "CommanderRules.h"
#include "StringUtils.h"

#include <unordered_map>

std::vector<ValidationIssue> DeckValidator::validate(
    const Deck& deck
)
{
    std::vector<ValidationIssue> issues;
    std::vector<Card> commanders;

    FormatRules rules =
        getFormatRules(deck.getFormat());

    int mainboardCount = 0;
    int sideboardCount = 0;
    int commanderCount = 0;

    std::unordered_map<std::string, int> cardCounts;
    std::unordered_map<std::string, bool> basicLandFlags;

    for (const auto& entry : deck.getEntries())
    {
        std::string section =
            toLower(entry.getSection());

        int quantity =
            entry.getQuantity();

        std::string cardName =
            toLower(entry.getCard().getName());

        std::string typeLine =
            toLower(entry.getCard().getTypeLine());

        if (BanListRepository::isBanned(
            entry.getCard().getName(),
            deck.getFormat()
        ))
        {
            issues.push_back({
                entry.getCard().getName()
                + " is banned in this format.",
                true
                });
        }

        bool isBasicLand =
            typeLine.find("basic land") != std::string::npos;

        basicLandFlags[cardName] = isBasicLand;

        if (section == "mainboard")
        {
            mainboardCount += quantity;
        }
        else if (section == "sideboard")
        {
            sideboardCount += quantity;
        }
        else if (section == "commander")
        {
            commanderCount += quantity;

            commanders.push_back(
                entry.getCard()
            );

            if (quantity != 1)
            {
                issues.push_back({
                    entry.getCard().getName()
                    + " has quantity "
                    + std::to_string(quantity)
                    + " in the Commander section; commander quantity must be 1.",
                    true
                    });
            }
        }

        cardCounts[cardName] += quantity;
    }

    if (
        deck.getFormat() == DeckFormat::Commander
        && !commanders.empty()
        )
    {
        std::string commanderIdentity =
            CommanderRules::getCombinedColorIdentity(
                commanders
            );

        for (const auto& entry : deck.getEntries())
        {
            if (
                toLower(entry.getSection())
                == "commander"
                )
            {
                continue;
            }

            const Card& card =
                entry.getCard();

            if (!CommanderRules::isWithinColorIdentity(
                card,
                commanderIdentity
            ))
            {
                issues.push_back({
                    card.getName()
                    + " has a color identity outside "
                    + "the commander's color identity.",
                    true
                    });
            }
        }
    }

    // Commander requirement
    if (rules.requiresCommander)
    {
        if (commanderCount == 0)
        {
            issues.push_back({
                "No commander selected.",
                true
                });
        }
        else if (commanderCount > 2)
        {
            issues.push_back({
                "Commander decks may not have more than two commanders.",
                true
                });
        }
    }

    // Commander section not allowed
    if (!rules.allowsCommander && commanderCount > 0)
    {
        issues.push_back({
            "This format does not allow a Commander section.",
            true
            });
    }

    // Minimum main deck size
    if (mainboardCount < rules.minimumMainDeckSize)
    {
        issues.push_back({
            "Main deck has "
            + std::to_string(mainboardCount)
            + " cards; minimum is "
            + std::to_string(rules.minimumMainDeckSize)
            + ".",
            false
            });
    }

    // Maximum main deck size
    if (
        rules.maximumMainDeckSize > 0
        &&
        mainboardCount > rules.maximumMainDeckSize
        )
    {
        issues.push_back({
            "Main deck has "
            + std::to_string(mainboardCount)
            + " cards; maximum is "
            + std::to_string(rules.maximumMainDeckSize)
            + ".",
            true
            });
    }

    // Sideboard limit
    if (deck.getFormat() == DeckFormat::Commander
        && sideboardCount > 0)
    {
        issues.push_back({
            "Commander does not use a standard sideboard. "
            "These cards are treated as considering cards.",
            false
            });
    }
    else if (sideboardCount > rules.maximumSideboardSize)
    {
        issues.push_back({
            "Sideboard has "
            + std::to_string(sideboardCount)
            + " cards; maximum is "
            + std::to_string(rules.maximumSideboardSize)
            + ".",
            true
            });
    }

    // Copy limit
    for (const auto& pair : cardCounts)
    {
        const std::string& cardName =
            pair.first;

        int count =
            pair.second;

        if (basicLandFlags[cardName]) 
        {
            continue;
        }

        if (count > rules.maximumCopiesPerCard)
        {
            issues.push_back({
                cardName
                + " appears "
                + std::to_string(count)
                + " times; maximum is "
                + std::to_string(rules.maximumCopiesPerCard)
                + ".",
                true
                });
        }
    }

    return issues;
}