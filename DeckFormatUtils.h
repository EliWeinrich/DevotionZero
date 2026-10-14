#pragma once

#include "DeckFormat.h"

#include <string>

std::string deckFormatToString(DeckFormat format);

DeckFormat stringToDeckFormat(
    const std::string& format
);