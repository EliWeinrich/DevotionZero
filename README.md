# Devotion Zero

Devotion Zero is a C++ Magic: The Gathering deck-building, validation,
and analytics application with Scryfall integration, format-aware rules,
Commander support, and MTGO import/export.

## Features

- Search for Magic cards using the Scryfall API
- Create and save multiple decks
- Supported formats:
  - Commander
  - Modern
  - Standard
  - Pauper
- Format-specific deck validation
- Commander support
  - Commander eligibility checking
  - Partner commanders
  - Friends Forever
  - Choose a Background
  - Doctor's companion
  - Commander color identity validation
- Banned-card validation
- Basic-land copy-limit exemptions
- Deck statistics
  - Total cards
  - Unique cards
  - Land/nonland counts
  - Average mana value
  - Average nonland mana value
- Deck organization by card type
- Alphabetical card sorting
- JSON persistence
- MTGO-format decklist export
- MTGO-format decklist import from:
  - Pasted text
  - `.txt` files

## Technologies

- C++
- Visual Studio
- libcurl
- nlohmann/json
- Scryfall API
- vcpkg

## Current Interface

Devotion Zero currently uses a console interface while the core deck-building and validation systems are developed.

A graphical interface using Qt is planned for a future version.

## Project Structure

- `Card` - card data model
- `DeckEntry` - card quantity and deck section
- `Deck` - deck model
- `ScryfallClient` - Scryfall API integration
- `DeckRepository` - JSON save/load persistence
- `DeckAnalyzer` - deck statistics
- `DeckValidator` - format validation
- `CommanderRules` - Commander-specific rules
- `FormatRules` - format construction rules
- `BanListRepository` - banned-card data
- `DeckOrganizer` - card grouping and sorting
- `DeckListIO` - MTGO import/export
- `DevotionZeroApp` - console application controller

## Roadmap

- Qt graphical interface
- Additional deck sorting options
- Improved Commander edge-case support
- Dynamic banned-list updates
- Additional Magic formats
- Improved Scryfall import performance
  - Batch card lookups using the Scryfall collection endpoint
  - Local card-data caching
  - Smarter API rate-limit and retry handling
- Advanced Deck Analytics Including:
	- Mana curve analysis
	- Color-source distribution
	- Land/spell ratios
	- Opening-hand statistics
	- Mulligan simulations
	- Draw probability calculations
	- Probability of seeing specific cards by a given turn
	- Combo-piece consistency analysis
	- Monte Carlo deck simulations
	- cEDH goldfish simulation
	  - Estimated turn-1 win percentage
	  - Estimated turn-2 win percentage
	  - Estimated turn-3 win percentage
	  - Uninterrupted / no-interaction assumptions

## Disclaimer

Devotion Zero is an unofficial fan-made project and is not affiliated with Wizards of the Coast.

Card data is provided by Scryfall.