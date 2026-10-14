#pragma once

#include "DeckFormat.h"

struct FormatRules
{
    int minimumMainDeckSize;
    int maximumMainDeckSize;

    int maximumSideboardSize;
    int maximumCopiesPerCard;

    bool requiresCommander;
    bool allowsCommander;

    bool singleton;
    bool enforceColorIdentity;
};

FormatRules getFormatRules(
    DeckFormat format
);