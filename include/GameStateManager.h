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
#include <qtypes.h>

class Game;

enum class savefile : qint16
{
    SUDOKU_SAVE_0 = 0,
    SUDOKU_SAVE_1,
    SUDOKU_SAVE_2,
    NO_NAME
};

class GameStateManager
{
public:
    static void saveGameState(const Game* game, const QGridLayout* gridLayout, quint32 seconds, savefile fileNum);
    static bool loadGameState(Game* game, QGridLayout* gridLayout, quint32& seconds, savefile fileNum);

private:
    static QString getFilePath(savefile file = savefile::SUDOKU_SAVE_0);
};