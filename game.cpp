#include "game.h"

Game::Game(QObject* parent)
    : QObject(parent),
      board(9, QVector<int>(9, 0))
{
    full_board = SudokuGenerator::getGeneratedSudoku();

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            init_block(i * 3, j * 3);
        }
    }
}

void Game::init_block(int i, int j)
{
    int count = 0;
    switch (difficulty)
    {
        case 0: {
            count = std::rand() % 3 + 4;
            break;
        }

        case 1: {
            count = std::rand() % 3 + 3;
        }

        case 2: {
            count = std::rand() % 3 + 2;
        }

        default: {
            count = 1;
        }
    }

    int index = 0;
    while (index != count)
    {
        int x = std::rand() % 3 + i;
        int y = std::rand() % 3 + j;

        if (board[x][y] == 0)
        {
            ++index;
            board[x][y] = full_board[x][y];
        }
    }
}

QVector<QVector<int>> Game::getBoard() const
{
    return board;
}