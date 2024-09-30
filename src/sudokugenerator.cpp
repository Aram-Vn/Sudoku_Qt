#include "../include/sudokugenerator.h"

QVector<QVector<int>> SudokuGenerator::getGeneratedSudoku()
{
    QVector<QVector<int>> board = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 7, 8, 9, 1, 2, 3, 4, 5, 6 },
                                    { 4, 5, 6, 7, 8, 9, 1, 2, 3 }, { 3, 1, 2, 6, 4, 5, 9, 7, 8 },
                                    { 9, 7, 8, 3, 1, 2, 6, 4, 5 }, { 6, 4, 5, 9, 7, 8, 3, 1, 2 },
                                    { 2, 3, 1, 5, 6, 4, 8, 9, 7 }, { 8, 9, 7, 2, 3, 1, 5, 6, 4 },
                                    { 5, 6, 4, 8, 9, 7, 2, 3, 1 } };

    QVector<void (*)(QVector<QVector<int>>&)> shuffle_func = {
        SudokuGenerator::mirrorX,          SudokuGenerator::mirrorY,   SudokuGenerator::clockwise,
        SudokuGenerator::counterClockwise, SudokuGenerator::transpose, SudokuGenerator::counterTranspose
    };

    QRandomGenerator* generator = QRandomGenerator::global();

    for (int i = 0; i < 10; ++i)
    {
        int index = generator->bounded(shuffle_func.size());
        shuffle_func[index](board);
    }

    shuffle_func = { shuffleRows, shuffleCols, shuffleRowBlocks, shuffleColBlocks };

    for (int i = 0; i < 15; ++i)
    {
        int index = generator->bounded(shuffle_func.size());
        shuffle_func[index](board);
    }

    return board;
}

void SudokuGenerator::mirrorX(QVector<QVector<int>>& board)
{
    std::reverse(board.begin(), board.end());
}

void SudokuGenerator::mirrorY(QVector<QVector<int>>& board)
{
    for (auto& vec : board)
    {
        std::reverse(vec.begin(), vec.end());
    }
}

void SudokuGenerator::clockwise(QVector<QVector<int>>& board)
{
    transpose(board);
    mirrorY(board);
}

void SudokuGenerator::counterClockwise(QVector<QVector<int>>& board)
{
    mirrorY(board);
    transpose(board);
}

void SudokuGenerator::transpose(QVector<QVector<int>>& board)
{
    int size = board.size();
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            std::swap(board[i][j], board[j][i]);
        }
    }
}

void SudokuGenerator::counterTranspose(QVector<QVector<int>>& board)
{
    int size = board.size();
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size - i - 1; ++j)
        {
            std::swap(board[i][j], board[size - j - 1][size - i - 1]);
        }
    }
}

void SudokuGenerator::shuffleRows(QVector<QVector<int>>& board)
{
    const int         block_size = 3;
    QRandomGenerator* generator  = QRandomGenerator::global();

    for (int i = 0; i < block_size; ++i)
    {
        QVector<int> shuffle_index(block_size, 0);

        for (int j = 0; j < block_size; ++j)
        {
            shuffle_index[j] = generator->bounded(block_size) + block_size * i;
        }

        std::swap(board[shuffle_index[0]], board[shuffle_index[1]]);
        std::swap(board[shuffle_index[1]], board[shuffle_index[2]]);
    }
}

void SudokuGenerator::shuffleCols(QVector<QVector<int>>& board)
{
    const int         block_size = 3;
    QRandomGenerator* generator  = QRandomGenerator::global();
    for (int i = 0; i < block_size; ++i)
    {
        QVector<int> shuffle_index(block_size, 0);

        for (int j = 0; j < block_size; ++j)
        {
            shuffle_index[j] = generator->bounded(block_size) + block_size * i;
        }

        for (int k = 0; k < board.size(); ++k)
        {
            std::swap(board[k][shuffle_index[0]], board[k][shuffle_index[1]]);
            std::swap(board[k][shuffle_index[1]], board[k][shuffle_index[2]]);
        }
    }
}

void SudokuGenerator::shuffleRowBlocks(QVector<QVector<int>>& board)
{
    const int         block_size = 3;
    QVector<int>      shuffle_index(block_size, 0);
    QRandomGenerator* generator = QRandomGenerator::global();
    for (int i = 0; i < block_size; ++i)
    {
        shuffle_index[i] = block_size * generator->bounded(block_size);
    }

    for (int i = 0; i < block_size; ++i)
    {
        std::swap(board[shuffle_index[0] + i], board[shuffle_index[1] + i]);
        std::swap(board[shuffle_index[1] + i], board[shuffle_index[2] + i]);
    }
}

void SudokuGenerator::shuffleColBlocks(QVector<QVector<int>>& board)
{
    const int         block_size = 3;
    QVector<int>      shuffle_index(block_size, 0);
    QRandomGenerator* generator = QRandomGenerator::global();

    for (int i = 0; i < block_size; ++i)
    {
        shuffle_index[i] = block_size * generator->bounded(block_size);
    }

    for (int i = 0; i < board.size(); ++i)
    {
        for (int j = 0; j < block_size; ++j)
        {
            std::swap(board[i][shuffle_index[0] + j], board[i][shuffle_index[1] + j]);
            std::swap(board[i][shuffle_index[1] + j], board[i][shuffle_index[2] + j]);
        }
    }
}