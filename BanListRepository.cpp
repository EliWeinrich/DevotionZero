#include "BanListRepository.h"
#include "StringUtils.h"

const std::unordered_set<std::string>&
BanListRepository::getBannedCards(
    DeckFormat format
)
{
    static const std::unordered_set<std::string>
        commanderBanned = {
        "ancestral recall",
        "balance",
        "black lotus",
        "chaos orb",
        "channel",
        "dockside extortionist",
        "emrakul, the aeons torn",
        "erayo, soratami ascendant",
        "falling star",
        "fastbond",
        "flash",
        "golos, tireless pilgrim",
        "griselbrand",
        "hullbreacher",
        "iona, shield of emeria",
        "karakas",
        "jeweled lotus",
        "leovold, emissary of trest",
        "library of alexandria",
        "limited resources",
        "mana crypt",
        "mox emerald",
        "mox jet",
        "mox pearl",
        "mox ruby",
        "mox sapphire",
        "nadu, winged wisdom",
        "paradox engine",
        "primeval titan",
        "prophet of kruphix",
        "recurring nightmare",
        "rofellos, llanowar emissary",
        "shahrazad",
        "sundering titan",
        "sylvan primordial",
        "time vault",
        "time walk",
        "tinker",
        "tolarian academy",
        "trade secrets",
        "upheaval",
        "yawgmoth's bargain"
    };

    static const std::unordered_set<std::string>
        modernBanned = {
         "amped raptor",
        "ancient den",
        "arcum's astrolabe",
        "birthing pod",
        "blazing shoal",
        "bridge from below",
        "chrome mox",
        "cloudpost",
        "dark depths",
        "deathrite shaman",
        "dig through time",
        "dread return",
        "eye of ugin",
        "field of the dead",
        "fury",
        "gitaxian probe",
        "glimpse of nature",
        "golgari grave-troll",
        "great furnace",
        "grief",
        "hogaak, arisen necropolis",
        "hypergenesis",
        "jegantha, the wellspring",
        "krark-clan ironworks",
        "lotus field",
        "lurrus of the dream-den",
        "mental misstep",
        "mycosynth lattice",
        "mystic sanctuary",
        "nadu, winged wisdom",
        "oko, thief of crowns",
        "once upon a time",
        "phlage, titan of fire's fury",
        "ponder",
        "punishing fire",
        "rite of flame",
        "seat of the synod",
        "second sunrise",
        "seething song",
        "sensei's divining top",
        "simian spirit guide",
        "skullclamp",
        "summer bloom",
        "the one ring",
        "tibalt's trickery",
        "treasure cruise",
        "tree of tales",
        "underworld breach",
        "up the beanstalk",
        "uro, titan of nature's wrath",
        "vault of whispers",
        "yorion, sky nomad"
    };

    static const std::unordered_set<std::string>
        standardBanned = {
        "abuelo's awakening",
        "badgermole cub",
        "cori-steel cutter",
        "gran-gran",
        "heartfire hero",
        "hopeless nightmare",
        "monstrous rage",
        "proft's eidetic memory",
        "screaming nemesis",
        "stormchaser's talent",
        "this town ain't big enough",
        "up the beanstalk",
        "vivi ornitier"
    };

    static const std::unordered_set<std::string>
        pauperBanned = {
        "aarakocra sneak",
        "all that glitters",
        "arcum's astrolabe",
        "atog",
        "basking broodscale",
        "chatterstorm",
        "cloud of faeries",
        "cloudpost",
        "cranial plating",
        "cranial ram",
        "daze",
        "deadly dispute",
        "disciple of the vault",
        "empty the warrens",
        "fall from favor",
        "frantic search",
        "galvanic relay",
        "gitaxian probe",
        "grapeshot",
        "gush",
        "high tide",
        "hymn to tourach",
        "invigorate",
        "kuldotha rebirth",
        "monastery swiftspear",
        "mystic sanctuary",
        "narcomoeba",
        "peregrine drake",
        "price of progress",
        "seeker of skybreak",
        "sinkhole",
        "sojourner's companion",
        "stirring bard",
        "temporal fissure",
        "treasure cruise",
        "underdark explorer",
        "vicious battlerager"
    };

    switch (format)
    {
    case DeckFormat::Commander:
        return commanderBanned;

    case DeckFormat::Modern:
        return modernBanned;

    case DeckFormat::Standard:
        return standardBanned;

    case DeckFormat::Pauper:
        return pauperBanned;
    }

    return commanderBanned;
}

bool BanListRepository::isBanned(
    const std::string& cardName,
    DeckFormat format
)
{
    const auto& bannedCards =
        getBannedCards(format);

    return bannedCards.find(
        toLower(cardName)
    ) != bannedCards.end();
}