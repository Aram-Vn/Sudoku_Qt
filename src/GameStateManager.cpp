#include "../include/GameStateManager.h"
#include <qdebug.h>
#include <qlogging.h>
#include <qobject.h>

void GameStateManager::saveGameState(const Game* game, const QGridLayout* gridLayout, quint32 seconds, savefile fileNum)
{
    QString filePath = getFilePath(fileNum);
    // qDebug() << "saveGameState ind: " << static_cast<int>(fileNum);
    // qDebug() << "saveGameState: " << filePath;

    QPushButton* darkStyleButton = dynamic_cast<QPushButton*>(gridLayout->itemAtPosition(0, 0)->widget());
    QString      darkStyle       = darkStyleButton->styleSheet();

    QPushButton* lightStyleButton = dynamic_cast<QPushButton*>(gridLayout->itemAtPosition(0, 4)->widget());
    QString      lightStyle       = lightStyleButton->styleSheet();

    QVector<QVector<int>> board     = game->getBoard();
    QVector<QVector<int>> fullBoard = game->getFullBoard();

    QVector<QVector<QString>> topRightButtonNumbers(9, QVector<QString>(9, QString()));
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            SudokuButton* button = dynamic_cast<SudokuButton*>(gridLayout->itemAtPosition(row, col)->widget());
            if (button)
            {
                topRightButtonNumbers[row][col] = button->getTopRightNumber();
            }
        }
    }

    fileUtil::writeInBinary(filePath, board, fullBoard, game->getDifficulty(), game->getEmptyCount(), game->getHearts(),
                            seconds, darkStyle, lightStyle, topRightButtonNumbers);
}

bool GameStateManager::loadGameState(Game* game, QGridLayout* gridLayout, quint32& seconds, savefile fileNum)
{
    QString filePath = getFilePath(fileNum);

    QVector<QVector<int>>     board;
    QVector<QVector<int>>     fullBoard;
    int                       difficulty;
    int                       emptyCount;
    int                       heartCount;
    QString                   darkStyle;
    QString                   lightStyle;
    QVector<QVector<QString>> topRightButtonNumbers;

    bool isSuccess = fileUtil::readFromBinary(filePath, board, fullBoard, difficulty, emptyCount, heartCount, seconds,
                                              darkStyle, lightStyle, topRightButtonNumbers);

    if (isSuccess)
    {
        game->setBoard(board);
        game->setFullBoard(fullBoard);
        game->setDifficulty(difficulty);
        game->setEmptyCount(emptyCount);
        game->setHearts(heartCount);

        colorUtil::applyColorStyles(gridLayout, darkStyle, lightStyle);

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                SudokuButton* button = dynamic_cast<SudokuButton*>(gridLayout->itemAtPosition(row, col)->widget());
                if (button)
                {
                    button->setTopRightNumber(topRightButtonNumbers[row][col]);
                }
            }
        }

        return true;
    }

    return false;
}

QString GameStateManager::getFilePath(savefile file)
{
    QString fileName;
    // qDebug() << "saveGameState: " << static_cast<int>(file);
    switch (file)
    {
        case savefile::SUDOKU_SAVE_0: fileName = "/sudoku_save_0.bin"; break;
        case savefile::SUDOKU_SAVE_1: fileName = "/sudoku_save_1.bin"; break;
        case savefile::SUDOKU_SAVE_2: fileName = "/sudoku_save_2.bin"; break;
        default: fileName = "NONAME";
    }
    // qDebug() << "fname " << fileName;

    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(filePath);
    return filePath.append(fileName);
}