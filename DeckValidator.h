#pragma once

#include "Deck.h"

#include <string>
#include <vector>

struct ValidationIssue
{
    std::string message;
    bool isError;
};

class DeckValidator
{
public:
    static std::vector<ValidationIssue> validate(
        const Deck& deck
    );
};