#pragma once

#include "SudokuButton/SudokuButton.h"
#include "game.h"
#include "utils/color_utils.h"
#include "utils/file_utils.h"

#include <QDir>
#include <QGridLayout>
#include <QStandardPaths>
#include <QString>
#include <QVector>
#include <qcontainerfwd.h>

class Game;

class GameStateManager
{
public:
    static void saveGameState(const Game* game, const QGridLayout* gridLayout, int seconds);
    static bool loadGameState(Game* game, QGridLayout* gridLayout, int& seconds);

private:
    static QString getFilePath(QString fileNae = "/sudoku_save.bin");
};