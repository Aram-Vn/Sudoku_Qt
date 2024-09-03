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
├── CMakeLists.txt          # CMake build configuration
├── include
│   ├── game.h              # Header file for game logic
│   ├── mainwindow.h        # Header file for main window UI
│   └── sudokugenerator.h   # Header file for Sudoku board generation
├── README.md               # Project overview and instructions
└── src
    ├── game.cpp            # Implementation of game logic
    ├── main.cpp            # Entry point for the application
    ├── mainwindow.cpp      # Implementation of the main window UI
    └── sudokugenerator.cpp # Implementation of Sudoku board generation

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

install it by using `make install` and use as from anywhere in terminal using `Sudoku` command

```bash
sudo make install

Sudoku # now you can call from anywhere
```

or 

3. Generate the Makefiles using CMake:

```bash
cmake ..
```

4. Build the project:

```bash
make
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