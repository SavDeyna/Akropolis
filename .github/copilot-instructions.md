# Akropolis Game Development Guidelines

## Architecture Overview
Akropolis is a Qt-based GUI application implementing the tile-laying board game. The core architecture follows a singleton pattern for game state management:

- **Partie** (singleton): Central game logic, manages participants, tiles, turns, and scoring
- **Plateau**: Hexagonal grid per player, handles tile placement and score calculation
- **Participation**: Links players to their individual boards and scores
- **UI Layers**: QStackedWidget in MainWindow switches between Menu, SelecJoueurs (player setup), and Jeu (game view)

Data flows from JSON files (`data/tuiles.json`, `data/mdj.json`) loaded into C++ objects at runtime.

## Key Patterns
- **Singleton for Game State**: Use `Partie::getInstance()` for global game access
- **Forward Declarations**: Heavily used in headers to avoid circular includes (e.g., `class Participation;`)
- **Qt Integration**: Mix C++ standard library with Qt containers; UI uses QWidget hierarchy
- **Hexagonal Coordinates**: Custom `HexagoneCoord` for grid positioning with q/r/s axes
- **Variant System**: Game modes support optional variants (Habitations, Marches, etc.) via `std::set<Variante>`

## Development Workflow
- **Build**: `cmake --build build/` (requires Qt6 installed)
- **Run**: Execute built binary with PATH including `C:/Qt/6.10.1/mingw_64/bin` and `C:/Qt/Tools/mingw1310_64/bin`
- **Debugging**: Use Qt Creator or VS Code with CMake tools; breakpoints in `Partie::debutTour()` for turn logic
- **Testing**: No automated tests present; manual testing via UI flows

## Code Conventions
- **File Structure**: Headers in `include/`, sources in `src/`, UI files at root
- **Naming**: PascalCase for classes (e.g., `ModeDeJeu`), camelCase for methods (e.g., `getParticipants()`)
- **Error Handling**: Exceptions for invalid tile constructions; no widespread try-catch
- **JSON Parsing**: Uses nlohmann/json for data loading; tiles defined as `[["color", number], ...]` arrays
- **Memory Management**: RAII with smart pointers minimal; manual memory in some areas

## Common Tasks
- **Adding Game Variants**: Modify `enum class Variante` and update `Plateau::calculValeur*()` methods
- **New UI Screens**: Extend `QStackedWidget` in `MainWindow`, add slots for navigation
- **Tile Generation**: Edit `data/tuiles.json` and reload via `Partie::ChargerTuiles()`
- **Scoring Logic**: Implement in `Plateau` private methods, called from `Partie::finTour()`

Reference: [include/Partie.h](include/Partie.h) for game flow, [include/Plateau.h](include/Plateau.h) for board mechanics</content>
<parameter name="filePath">c:\Users\clere\Documents\Etudes\UTC\GI01\LO21\Akropolis\.github\copilot-instructions.md