#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Card
{
private:
    std::string name;
    double manaValue;
    std::string manaCost;
    std::string typeLine;
    std::string oracleText;
    std::string colorIdentity;

public:
    Card();
    Card(
        const std::string& name,
        double manaValue,
        const std::string& manaCost,
        const std::string& typeLine,
        const std::string& oracleText,
        const std::string& colorIdentity
    );

    const std::string& getName() const;
    double getManaValue() const;
    const std::string& getTypeLine() const;
    const std::string& getColorIdentity() const;
    const std::string& getManaCost() const;
    const std::string& getOracleText() const;

    friend void to_json(nlohmann::json& j, const Card& card);
    friend void from_json(const nlohmann::json& j, Card& card);
};