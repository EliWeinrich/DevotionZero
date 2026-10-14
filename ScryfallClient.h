#pragma once

#include "Card.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <nlohmann/json.hpp>

class ScryfallRateLimitException
    : public std::runtime_error
{
public:
    ScryfallRateLimitException()
        : std::runtime_error(
            "Scryfall rate limit reached (HTTP 429)."
        )
    {
    }
};

class ScryfallClient
{
public:
    Card fetchCardByName(
        const std::string& cardName
    ) const;

    std::vector<std::string> autocompleteCardNames(
        const std::string& query
    ) const;

    std::vector<Card> fetchCardsByNames(
        const std::vector<std::string>& cardNames
    ) const;

    Card fetchCardByFuzzyName(
        const std::string& cardName
    ) const;

private:
    std::string performRequest(
        const std::string& url
    ) const;

    std::string urlEncode(
        const std::string& value
    ) const;

    std::string performPostRequest(
        const std::string& url,
        const std::string& body
    ) const;

    Card cardFromJson(
        const nlohmann::json& data
    ) const;
};