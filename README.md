# ProjetJeuxEchecs — macOS / VS Code setup

This repository was migrated from Visual Studio (Windows) to macOS + VS Code. The project now uses a tidy layout and CMake.

Quick start (Homebrew Qt):

```bash
# Install Qt via Homebrew (if needed)
brew install qt

# Configure and build (out-of-source)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/ProjetJeuxEchecs
```

Notes

Project layout

If you want, I can also add a `.vscode` folder with CMake Tools and launch configurations.

# ProjetJeuxEchecs

ProjetJeuxEchecs is a small chess game project with a Qt GUI and separable game logic. This repository was migrated from Visual Studio (Windows) to macOS + VS Code and uses CMake for builds.

**What it contains**

- GUI application: the Qt-based executable `ProjetJeuxEchecs` (entry: `src/main.cpp`).
- Game logic and helpers in the sources under `src/` and headers under `include/`.
- Resources/UI: `resources/ProjetJeuxEchecs.ui` and `resources/ProjetJeuxEchecs.qrc`.
- Vendor/testing helpers: `bibliotheque_cours/` and `googletest/` remain in-tree for convenience.
- Visual Studio artifacts were moved to `visual_studio/` for historical reference.

Features

- Qt6-based GUI (Core + Widgets).
- Modular code layout so game logic can be reused/tested without Qt.
- CMake AUTOUIC/AUTORCC/AUTOMOC enabled — `.ui` and `.qrc` processed automatically.

Prerequisites (macOS)

- Xcode command line tools (compiler)
- CMake >= 3.10
- Qt 6 (install with Homebrew or the official Qt installer)

Install Qt (recommended via Homebrew)

```bash
brew update
brew install qt
```

Configure, build and run (out-of-source)

```bash
# From repository root
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/ProjetJeuxEchecs
```

Notes on Qt installation

- If you installed Qt with the official online installer, pass the CMake prefix path:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/Qt/<version>/lib/cmake
```

or set `-DQt6_DIR=/path/to/Qt/<version>/lib/cmake/Qt6`.

Running tests

- This project includes GoogleTest sources; if you add a separate test target in CMake you can build/run it with:

```bash
cmake --build build --target tests
./build/tests
```

Currently the main executable may run any built-in test runner depending on build-time options (see `bibliotheque_cours`).

Project layout (recommended)

- `src/` — implementation (.cpp)
- `include/` — public headers
- `resources/` — `.ui`, `.qrc`, icons
- `cmake/` — CMake helper modules (now contains `cmake_cours.cmake`)
- `visual_studio/` — original VS solution/project files (kept for history)
- `googletest/`, `bibliotheque_cours/` — third-party/vendor code

Cleaning build

```bash
rm -rf build
```

Want help

- I can: add a `lib/` vs `app/` split and update `CMakeLists.txt`, add a `.vscode/` CMake Tools + launch configuration, or create a test target. Which should I do next?
