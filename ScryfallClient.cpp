#include "ScryfallClient.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

namespace
{
    size_t writeCallback(
        void* contents,
        size_t size,
        size_t nmemb,
        void* userp
    )
    {
        size_t totalSize = size * nmemb;

        std::string* response =
            static_cast<std::string*>(userp);

        response->append(
            static_cast<char*>(contents),
            totalSize
        );

        return totalSize;
    }
}


std::string ScryfallClient::urlEncode(
    const std::string& value
) const
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        throw std::runtime_error(
            "Failed to initialize curl."
        );
    }

    char* encoded =
        curl_easy_escape(
            curl,
            value.c_str(),
            static_cast<int>(value.length())
        );

    if (!encoded)
    {
        curl_easy_cleanup(curl);

        throw std::runtime_error(
            "Failed to encode card name."
        );
    }

    std::string result(encoded);

    curl_free(encoded);
    curl_easy_cleanup(curl);

    return result;
}

std::string ScryfallClient::performRequest(
    const std::string& url
) const
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        throw std::runtime_error(
            "Failed to initialize curl."
        );
    }

    std::string response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "DevotionZero/0.1"
    );

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Accept: application/json;q=0.9,*/*;q=0.8"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    CURLcode result =
        curl_easy_perform(curl);

    long httpCode = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &httpCode
    );

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        throw std::runtime_error(
            "Scryfall request failed."
        );
    }

    if (httpCode == 429)
    {
        throw ScryfallRateLimitException();
    }

    if (httpCode != 200)
    {
        throw std::runtime_error(
            "Scryfall returned HTTP status "
            + std::to_string(httpCode)
        );
    }

    return response;
}

std::string ScryfallClient::performPostRequest(
    const std::string& url,
    const std::string& body
) const
{
    CURL* curl =
        curl_easy_init();

    if (!curl)
    {
        throw std::runtime_error(
            "Failed to initialize CURL."
        );
    }

    std::string response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POST,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        body.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "DevotionZero/0.1"
    );

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Accept: application/json;q=0.9,*/*;q=0.8"
    );

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    CURLcode result =
        curl_easy_perform(curl);

    long httpCode = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &httpCode
    );

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        throw std::runtime_error(
            "Scryfall request failed."
        );
    }

    if (httpCode == 429)
    {
        throw ScryfallRateLimitException();
    }

    if (httpCode != 200)
    {
        throw std::runtime_error(
            "Scryfall returned HTTP status "
            + std::to_string(httpCode)
        );
    }

    return response;
}

Card ScryfallClient::cardFromJson(
    const nlohmann::json& data
) const
{
    std::string name =
        data.at("name").get<std::string>();

    double manaValue =
        data.at("cmc").get<double>();

    std::string manaCost =
        data.value("mana_cost", "");

    std::string typeLine =
        data.value("type_line", "");

    std::string oracleText =
        data.value("oracle_text", "");

    std::string colorIdentity;

    if (data.contains("color_identity"))
    {
        for (const auto& color :
            data["color_identity"])
        {
            colorIdentity +=
                color.get<std::string>();
        }
    }

    return Card(
        name,
        manaValue,
        manaCost,
        typeLine,
        oracleText,
        colorIdentity
    );
}

Card ScryfallClient::fetchCardByName(
    const std::string& cardName
) const
{
    std::string encodedName =
        urlEncode(cardName);

    std::string url =
        "https://api.scryfall.com/cards/named?exact="
        + encodedName;

    std::string response =
        performRequest(url);

    nlohmann::json data =
        nlohmann::json::parse(response);

    return cardFromJson(data);
}

std::vector<Card> ScryfallClient::fetchCardsByNames(
    const std::vector<std::string>& cardNames
) const
{
    std::vector<Card> cards;

    const std::size_t batchSize = 75;

    for (std::size_t start = 0;
        start < cardNames.size();
        start += batchSize)
    {
        nlohmann::json body;

        body["identifiers"] =
            nlohmann::json::array();

        std::size_t end =
            start + batchSize;

        if (end > cardNames.size())
        {
            end = cardNames.size();
        }

        for (std::size_t i = start;
            i < end;
            ++i)
        {
            body["identifiers"].push_back({
                {"name", cardNames[i]}
                });
        }

        std::string response =
            performPostRequest(
                "https://api.scryfall.com/cards/collection",
                body.dump()
            );

        nlohmann::json result =
            nlohmann::json::parse(response);

        if (result.contains("data"))
        {
            for (const auto& cardData :
                result["data"])
            {
                cards.push_back(
                    cardFromJson(cardData)
                );
            }
        }

        // Only relevant for decks over 75 unique names.
        if (end < cardNames.size())
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(200)
            );
        }
    }

    return cards;
}

std::vector<std::string> ScryfallClient::autocompleteCardNames(
    const std::string& query
) const
{
    std::string encodedQuery = urlEncode(query);

    std::string url =
        "https://api.scryfall.com/cards/autocomplete?q="
        + encodedQuery;

    std::string response = performRequest(url);

    json data = json::parse(response);

    std::vector<std::string> names;

    if (!data.contains("data"))
    {
        return names;
    }

    for (const auto& item : data["data"])
    {
        names.push_back(item.get<std::string>());
    }

    return names;
}

Card ScryfallClient::fetchCardByFuzzyName(
    const std::string& cardName
) const
{
    std::string encodedName =
        urlEncode(cardName);

    std::string url =
        "https://api.scryfall.com/cards/named?fuzzy="
        + encodedName;

    std::string response =
        performRequest(url);

    nlohmann::json data =
        nlohmann::json::parse(response);

    return cardFromJson(data);
}