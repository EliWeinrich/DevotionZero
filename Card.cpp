#include "Card.h"
#include <nlohmann/json.hpp>

Card::Card()
    : name(""),
    manaValue(0.0),
    manaCost(""),
    typeLine(""),
    oracleText(""),
    colorIdentity("")
{
}

Card::Card(
    const std::string& name,
    double manaValue,
    const std::string& manaCost,
    const std::string& typeLine,
    const std::string& oracleText,
    const std::string& colorIdentity
)
    : name(name),
    manaValue(manaValue),
    manaCost(manaCost),
    typeLine(typeLine),
    oracleText(oracleText),
    colorIdentity(colorIdentity)
{
}

const std::string& Card::getManaCost() const
{
    return manaCost;
}

const std::string& Card::getOracleText() const
{
    return oracleText;
}

const std::string& Card::getName() const
{
    return name;
}

double Card::getManaValue() const
{
    return manaValue;
}

const std::string& Card::getTypeLine() const
{
    return typeLine;
}

const std::string& Card::getColorIdentity() const
{
    return colorIdentity;
}

void to_json(nlohmann::json& j, const Card& card)
{
    j = nlohmann::json{
        {"name", card.name},
        {"manaValue", card.manaValue},
        {"manaCost", card.manaCost},
        {"typeLine", card.typeLine},
        {"oracleText", card.oracleText},
        {"colorIdentity", card.colorIdentity}
    };
}

void from_json(const nlohmann::json& j, Card& card)
{
    j.at("name").get_to(card.name);
    j.at("manaValue").get_to(card.manaValue);
    j.at("typeLine").get_to(card.typeLine);
    j.at("colorIdentity").get_to(card.colorIdentity);

    card.manaCost =
        j.value("manaCost", "");

    card.oracleText =
        j.value("oracleText", "");
}