#include "game.h"

Game::Game(QObject* parent)
    : QObject(parent),
      m_board(9, QVector<int>(9, 0)),
      m_x{ -1 },
      m_y{ -1 }
{
}

void Game::initGame()
{
    m_hearts       = 3;
    m_empty_fields = 0;
    m_full_board   = SudokuGenerator::getGeneratedSudoku();

    for (int i = 0; i < m_board.size(); ++i)
    {
        for (int j = 0; j < m_board.size(); ++j)
        {
            m_board[i][j] = 0;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            init_block(i * 3, j * 3);
        }
    }

    emit board_is_ready();
}

void Game::numberEvent(int number)
{
    if (m_x != -1)
    {
        if (m_full_board[m_x][m_y] == number)
        {
            m_board[m_x][m_y] = number;
            --m_empty_fields;
            emit add_on_grid();
        }

        else
        {
            --m_hearts;
            emit change_hearts_count();
        }
    }
}

void Game::init_block(int i, int j)
{
    int count = 0;
    switch (m_difficulty)
    {
        case 0: {
            count = std::rand() % 3 + 4;
            break;
        }

        case 1: {
            count = std::rand() % 3 + 3;
            break;
        }

        case 2: {
            count = std::rand() % 3 + 2;
            break;
        }

        default: {
            count = 1;
        }
    }

    m_empty_fields += 9 - count;

    int index = 0;
    while (index != count)
    {
        int x = std::rand() % 3 + i;
        int y = std::rand() % 3 + j;

        if (m_board[x][y] == 0)
        {
            ++index;
            m_board[x][y] = m_full_board[x][y];
        }
    }
}

QVector<QVector<int>> Game::getBoard() const
{
    return m_board;
}

void Game::setDifficulty(int diff)
{
    m_difficulty = diff;
}

void Game::setCoords(int i, int j)
{
    m_x = i;
    m_y = j;
}

int Game::getX() const
{
    return m_x;
}

int Game::getY() const
{
    return m_y;
}

int Game::getNumber(int i, int j) const
{
    return m_board[i][j];
}

int Game::getHearts() const
{
    return m_hearts;
}

int Game::getEmptyCount() const
{
    return m_empty_fields;
}

bool Game::checkPos(int i, int j) const
{
    return !m_board[i][j];
}