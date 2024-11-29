#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace constants {
    // Font Sizes
    constexpr int DIFFICULTY_BUTTONS_FONT_SIZE = 16;
    constexpr int SUDOKU_BUTTON_FONT_SIZE      = 24;

    // Game Configuration
    constexpr int DEFAULT_HEART_COUNT          = 3;
    constexpr int DEFAULT_HEART_WIDTH_HEIGHT   = 22;
    const QString DEFAULT_HEART_IMAGE_PATH     = ":/assets/Heart.png";
    const QString DEFAULT_SAVE_ICON_IMAGE_PATH = ":/assets/save-icon.png";

    // Window Size
    constexpr int WINDOW_WIDTH  = 700;
    constexpr int WINDOW_HEIGHT = 850;

    // Sudoku Grid Configuration
    constexpr int GRID_SIZE          = 9;
    constexpr int SUDOKU_BUTTON_SIZE = 60;
    constexpr int BUTTON_HEIGHT      = 50;
} // namespace constants

#endif // CONSTANTS_H
