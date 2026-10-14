#include "FormatRules.h"

FormatRules getFormatRules(
    DeckFormat format
)
{
    switch (format)
    {
    case DeckFormat::Commander:
        return FormatRules{
            99,     // minimum main deck size
            99,     // maximum main deck size
            0,      // sideboard size
            1,      // copies per card
            true,   // requires commander
            true,   // allows commander
            true,   // singleton
            true    // color identity
        };

    case DeckFormat::Modern:
        return FormatRules{
            60,
            0,      // 0 = no maximum
            15,
            4,
            false,
            false,
            false,
            false
        };

    case DeckFormat::Standard:
        return FormatRules{
            60,
            0,
            15,
            4,
            false,
            false,
            false,
            false
        };

    case DeckFormat::Pauper:
        return FormatRules{
            60,
            0,
            15,
            4,
            false,
            false,
            false,
            false
        };
    }

    return FormatRules{};
}