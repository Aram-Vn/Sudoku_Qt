#include "../include/GameStateManager.h"

void GameStateManager::saveGameState(const Game* game, const QGridLayout* gridLayout, int seconds)
{
    QString filePath = getFilePath();

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

bool GameStateManager::loadGameState(Game* game, QGridLayout* gridLayout, int& seconds)
{
    QString filePath = getFilePath();

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

QString GameStateManager::getFilePath(QString fileNae)
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(filePath);
    return filePath.append(fileNae);
}