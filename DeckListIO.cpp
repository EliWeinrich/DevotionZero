#include "DeckListIO.h"
#include "StringUtils.h"

#include <algorithm>
#include <vector>
#include <fstream> 
#include <sstream> 


bool DeckListIO::exportMTGO(
    const Deck& deck,
    const std::string& filename
)
{
    std::ofstream outFile(filename);

    if (!outFile)
    {
        return false;
    }

    std::vector<DeckEntry> mainboard;
    std::vector<DeckEntry> sideboard;
    std::vector<DeckEntry> commanders;

    for (const auto& entry : deck.getEntries())
    {
        std::string section =
            toLower(entry.getSection());

        if (section == "mainboard")
        {
            mainboard.push_back(entry);
        }
        else if (section == "sideboard")
        {
            sideboard.push_back(entry);
        }
        else if (section == "commander")
        {
            commanders.push_back(entry);
        }
    }

    auto sortAlphabetically =
        [](std::vector<DeckEntry>& entries)
        {
            std::sort(
                entries.begin(),
                entries.end(),
                [](const DeckEntry& a, const DeckEntry& b)
                {
                    return toLower(
                        a.getCard().getName()
                    )
                        <
                        toLower(
                            b.getCard().getName()
                        );
                }
            );
        };

    sortAlphabetically(mainboard);
    sortAlphabetically(sideboard);
    sortAlphabetically(commanders);

    // Mainboard
    for (const auto& entry : mainboard)
    {
        outFile
            << entry.getQuantity()
            << " "
            << entry.getCard().getName()
            << "\n";
    }

    // Sideboard
    if (!sideboard.empty())
    {
        outFile << "\nSIDEBOARD:\n";

        for (const auto& entry : sideboard)
        {
            outFile
                << entry.getQuantity()
                << " "
                << entry.getCard().getName()
                << "\n";
        }
    }

    // Commander / Commanders
    if (!commanders.empty())
    {
        outFile << "\n";

        for (const auto& entry : commanders)
        {
            outFile
                << entry.getQuantity()
                << " "
                << entry.getCard().getName()
                << "\n";
        }
    }

    return true;
}

std::string DeckListIO::readTextFile(
    const std::string& filename
)
{
    std::string cleanFilename = filename;

    if (
        cleanFilename.size() >= 2
        && cleanFilename.front() == '"'
        && cleanFilename.back() == '"'
        )
    {
        cleanFilename =
            cleanFilename.substr(
                1,
                cleanFilename.size() - 2
            );
    }

    std::ifstream inFile(cleanFilename);

    if (!inFile)
    {
        return "";
    }

    std::ostringstream buffer;

    buffer << inFile.rdbuf();

    return buffer.str();
}

std::vector<ImportedDeckEntry>
DeckListIO::parseMTGO(
    const std::string& deckText,
    DeckFormat format
)
{
    std::vector<ImportedDeckEntry> importedEntries;

    std::istringstream input(deckText);

    std::vector<std::vector<std::string>> blocks;
    std::vector<std::string> currentBlock;

    std::string line;

    // First split the text into blocks separated
    // by blank lines.
    while (std::getline(input, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            if (!currentBlock.empty())
            {
                blocks.push_back(currentBlock);
                currentBlock.clear();
            }

            continue;
        }

        currentBlock.push_back(line);
    }

    if (!currentBlock.empty())
    {
        blocks.push_back(currentBlock);
    }

    bool explicitCommanderSection = false;

    for (const auto& block : blocks)
    {
        for (const std::string& blockLine : block)
        {
            std::string lowerLine =
                toLower(blockLine);

            if (
                lowerLine == "commander:"
                || lowerLine == "commander"
                )
            {
                explicitCommanderSection = true;
            }
        }
    }

    for (std::size_t blockIndex = 0;
        blockIndex < blocks.size();
        ++blockIndex)
    {
        std::string section = "Mainboard";

        const auto& block =
            blocks[blockIndex];

        bool sideboardBlock = false;
        bool commanderBlock = false;

        for (const std::string& blockLine : block)
        {
            std::string lowerLine =
                toLower(blockLine);

            if (
                lowerLine == "sideboard:"
                || lowerLine == "sideboard"
                )
            {
                sideboardBlock = true;
            }

            if (
                lowerLine == "commander:"
                || lowerLine == "commander"
                )
            {
                commanderBlock = true;
            }
        }

        if (sideboardBlock)
        {
            section = "Sideboard";
        }
        else if (commanderBlock)
        {
            section = "Commander";
        }
        else if (
            format == DeckFormat::Commander
            && !explicitCommanderSection
            && blockIndex == blocks.size() - 1
            && blockIndex > 0
            && block.size() <= 2
            )
        {
            // Moxfield-style Commander export:
            // final blank-separated block contains
            // one or two commanders without a heading.
            section = "Commander";
        }

        for (const std::string& blockLine : block)
        {
            std::string lowerLine =
                toLower(blockLine);

            if (
                lowerLine == "sideboard:"
                || lowerLine == "sideboard"
                || lowerLine == "commander:"
                || lowerLine == "commander"
                )
            {
                continue;
            }

            std::istringstream lineStream(
                blockLine
            );

            int quantity;

            if (!(lineStream >> quantity))
            {
                continue;
            }

            std::string cardName;

            std::getline(
                lineStream,
                cardName
            );

            // Remove the space left after reading quantity.
            if (
                !cardName.empty()
                && cardName.front() == ' '
                )
            {
                cardName.erase(
                    cardName.begin()
                );
            }

            if (
                quantity <= 0
                || cardName.empty()
                )
            {
                continue;
            }

            importedEntries.push_back({
                quantity,
                cardName,
                section
                });
        }
    }

    return importedEntries;
}