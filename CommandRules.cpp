#include "CommanderRules.h"
#include "StringUtils.h"

bool CommanderRules::canBeCommander(
    const Card& card
)
{
    std::string typeLine =
        toLower(card.getTypeLine());

    std::string oracleText =
        toLower(card.getOracleText());

    bool isLegendaryCreature =
        typeLine.find("legendary creature")
        != std::string::npos;

    bool explicitlyAllowed =
        oracleText.find("can be your commander")
        != std::string::npos;

    return isLegendaryCreature || explicitlyAllowed;
}

bool CommanderRules::canPartnerTogether(
    const Card& first,
    const Card& second
)
{
    std::string firstText =
        toLower(first.getOracleText());

    std::string secondText =
        toLower(second.getOracleText());

    std::string firstType =
        toLower(first.getTypeLine());

    std::string secondType =
        toLower(second.getTypeLine());

    std::string firstName =
        toLower(first.getName());

    std::string secondName =
        toLower(second.getName());

    bool firstGenericPartner =
        firstText.find("partner") != std::string::npos
        &&
        firstText.find("partner with") == std::string::npos;

    bool secondGenericPartner =
        secondText.find("partner") != std::string::npos
        &&
        secondText.find("partner with") == std::string::npos;

    if (
        firstGenericPartner
        && secondGenericPartner
        )
    {
        return true;
    }

    bool firstPartnersWithSecond =
        firstText.find(
            "partner with " + secondName
        ) != std::string::npos;

    bool secondPartnersWithFirst =
        secondText.find(
            "partner with " + firstName
        ) != std::string::npos;

    if (
        firstPartnersWithSecond
        || secondPartnersWithFirst
        )
    {
        return true;
    }

    bool firstFriendsForever =
        firstText.find("friends forever")
        != std::string::npos;

    bool secondFriendsForever =
        secondText.find("friends forever")
        != std::string::npos;

    if (firstFriendsForever && secondFriendsForever)
    {
        return true;
    }

    bool firstChoosesBackground =
        firstText.find("choose a background")
        != std::string::npos;

    bool secondChoosesBackground =
        secondText.find("choose a background")
        != std::string::npos;

    bool firstIsBackground =
        firstType.find("background")
        != std::string::npos;

    bool secondIsBackground =
        secondType.find("background")
        != std::string::npos;

    if (
        firstChoosesBackground
        && secondIsBackground
        )
    {
        return true;
    }

    if (
        secondChoosesBackground
        && firstIsBackground
        )
    {
        return true;
    }

    bool firstDoctorsCompanion =
        firstText.find("doctor's companion")
        != std::string::npos;

    bool secondDoctorsCompanion =
        secondText.find("doctor's companion")
        != std::string::npos;

    bool firstIsDoctor =
        firstType.find("doctor")
        != std::string::npos;

    bool secondIsDoctor =
        secondType.find("doctor")
        != std::string::npos;

    if (
        firstDoctorsCompanion
        && secondIsDoctor
        )
    {
        return true;
    }

    if (
        secondDoctorsCompanion
        && firstIsDoctor
        )
    {
        return true;
    }

    return false;
}

std::string CommanderRules::getCombinedColorIdentity(
    const std::vector<Card>& commanders
)
{
    std::string combinedIdentity;

    for (const Card& commander : commanders)
    {
        for (char color : commander.getColorIdentity())
        {
            if (combinedIdentity.find(color)
                == std::string::npos)
            {
                combinedIdentity += color;
            }
        }
    }

    return combinedIdentity;
}

bool CommanderRules::isWithinColorIdentity(
    const Card& card,
    const std::string& commanderIdentity
)
{
    for (char color : card.getColorIdentity())
    {
        if (commanderIdentity.find(color)
            == std::string::npos)
        {
            return false;
        }
    }

    return true;
}