# Sudoku_Qt

- [Preview](#preview)
- [Overview](#overview)
- [Project Structure](#project-structure)
- [Features](#features)
- [Requirements](#requirements)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Acknowledgements](#acknowledgements)

## Preview

<div align="center">
  <img src="https://github.com/user-attachments/assets/40c8312a-1d35-442b-aea8-afce0f7a320f" width="500" height="auto" alt="sudoku">
</div>

## Overview

This project is a Sudoku game implemented using C++ and the Qt framework. It features a graphical user interface (GUI) that allows users to play Sudoku with varying difficulty levels. Users can also save and load game states, change button colors, and reset the game.

## Project Structure

```bash
.
│
├── include/
│   │
│   │
│   ├── game.h
│   ├── mainwindow.h
│   ├── sudokugenerator.h
│   │
│   ├── custombutton/
│   │   └── custombutton.h
│   │
│   └── utils/
│       ├── color_utils.h
│       └── file_utils.h
│
├── src/
│   │
│   ├── game.cpp
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── sudokugenerator.cpp
│   │
│   ├── custombutton/
│   │   └── custombutton.cpp
│   │
│   └── utils/
│       ├── color_utils.cpp
│       └── file_utils.cpp
│
├── .clang-format
├── CMakeLists.txt
├── README.md
├── .gitignore
└── .valgrind-suppressions

```

## Features

- **Sudoku Board**: Generates and displays a Sudoku board with a 9x9 grid.
- **Difficulty Levels**: Select from Easy, Medium, or Hard difficulty levels.
- **Game State**: Save and load game states to continue previous sessions.
- **Color Customization**: Choose colors for Sudoku buttons.
- **Timer**: Tracks the time taken to solve the Sudoku puzzle.
- **Heart System**: Indicates the number of remaining chances.


## Requirements

- C++11 or later
- Qt 5.15 or later
- CMake 3.10 or later

## Build Instructions

1. Clone the repository:

```bash
git clone https://github.com/your-username/sudoku-game.git
cd sudoku-game
```

2. Create a build directory and navigate into it:

```bash
mkdir build
cd build
```

3. Generate the Makefiles using CMake:

```bash
cmake ..
```

4. Build the project:

```bash
cmake --build .
```

5. Run the application:

```bash
./sudoku_game
```

## Usage

1. Start a New Game: Select a difficulty level and click "Start" to begin a new game.
2. Continue Old Game: Click "Continue old game" to resume a previously saved game.
3. Reset Game: Click "Reset" to reset the current game and start over.
4. Choose Colors: Click "Choose Color" to pick colors for Sudoku buttons.

## Acknowledgements

- Qt Framework: Provides the GUI components for the application.
- CMake: Used for building the project.