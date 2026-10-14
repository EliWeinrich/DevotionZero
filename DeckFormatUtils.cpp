#include "DeckFormatUtils.h"
#include "StringUtils.h"

std::string deckFormatToString(
    DeckFormat format
)
{
    switch (format)
    {
    case DeckFormat::Commander:
        return "Commander";

    case DeckFormat::Modern:
        return "Modern";

    case DeckFormat::Standard:
        return "Standard";

    case DeckFormat::Pauper:
        return "Pauper";
    }

    return "Unknown";
}

DeckFormat stringToDeckFormat(
    const std::string& format
)
{
    std::string lowerFormat =
        toLower(format);

    if (lowerFormat == "commander")
        return DeckFormat::Commander;

    if (lowerFormat == "modern")
        return DeckFormat::Modern;

    if (lowerFormat == "standard")
        return DeckFormat::Standard;

    if (lowerFormat == "pauper")
        return DeckFormat::Pauper;

    return DeckFormat::Commander;
}