#include "DeckOrganizer.h"
#include "StringUtils.h"
#include <algorithm>

std::string DeckOrganizer::getGroupName(
    const DeckEntry& entry
)
{
    std::string section =
        toLower(entry.getSection());

    std::string typeLine =
        toLower(
            entry.getCard().getTypeLine()
        );

    // Commanders are always separate
    if (section == "commander")
    {
        return "Commanders";
    }

    // Sideboard cards are always separate
    if (section == "sideboard")
    {
        return "Sideboard";
    }

    // Lands override other card types
    if (
        typeLine.find("land")
        != std::string::npos
        )
    {
        return "Lands";
    }

    if (
        typeLine.find("creature")
        != std::string::npos
        )
    {
        return "Creatures";
    }

    if (
        typeLine.find("planeswalker")
        != std::string::npos
        )
    {
        return "Planeswalkers";
    }

    if (
        typeLine.find("artifact")
        != std::string::npos
        )
    {
        return "Artifacts";
    }

    if (
        typeLine.find("enchantment")
        != std::string::npos
        )
    {
        return "Enchantments";
    }

    if (
        typeLine.find("sorcery")
        != std::string::npos
        )
    {
        return "Sorceries";
    }

    if (
        typeLine.find("instant")
        != std::string::npos
        )
    {
        return "Instants";
    }

    if (
        typeLine.find("battle")
        != std::string::npos
        )
    {
        return "Battles";
    }

    return "Other";
}

std::map<
    std::string,
    std::vector<DeckEntry>
> DeckOrganizer::groupEntries(
    const Deck& deck
)
{
    std::map<
        std::string,
        std::vector<DeckEntry>
    > groups;

    for (const auto& entry : deck.getEntries())
    {
        std::string groupName =
            getGroupName(entry);

        groups[groupName].push_back(entry);
    }

    for (auto& pair : groups)
    {
        std::vector<DeckEntry>& entries =
            pair.second;

        std::sort(
            entries.begin(),
            entries.end(),
            [](const DeckEntry& a, const DeckEntry& b)
            {
                return toLower(
                    a.getCard().getName()
                )
                    <
                    toLower(
                        b.getCard().getName()
                    );
            }
        );
    }

    return groups;
}