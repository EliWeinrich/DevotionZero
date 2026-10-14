#pragma once

#include "Card.h"

#include <vector>
#include <string>

class CommanderRules
{
public:
    static bool canBeCommander(
        const Card& card
    );

    static bool canPartnerTogether(
        const Card& first,
        const Card& second
    );

    static std::string getCombinedColorIdentity(
        const std::vector<Card>& commanders
    );

    static bool isWithinColorIdentity(
        const Card& card,
        const std::string& commanderIdentity
    );
};