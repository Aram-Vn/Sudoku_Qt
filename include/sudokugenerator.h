#ifndef SUDOKU_QT_INCLUDE_SUDOKU_GENERATOR_H
#define SUDOKU_QT_INCLUDE_SUDOKU_GENERATOR_H

#include <QRandomGenerator>
#include <QVector>

class SudokuGenerator
{
private:
    static void mirrorX(QVector<QVector<int>>& board);
    static void mirrorY(QVector<QVector<int>>& board);
    static void clockwise(QVector<QVector<int>>& board);
    static void counterClockwise(QVector<QVector<int>>& board);
    static void transpose(QVector<QVector<int>>& board);
    static void counterTranspose(QVector<QVector<int>>& board);

    static void shuffleRows(QVector<QVector<int>>& board);
    static void shuffleCols(QVector<QVector<int>>& board);
    static void shuffleRowBlocks(QVector<QVector<int>>& board);
    static void shuffleColBlocks(QVector<QVector<int>>& board);

public:
    SudokuGenerator()  = default;
    ~SudokuGenerator() = default;

    static QVector<QVector<int>> getGeneratedSudoku();
};

#endif // SUDOKU_QT_INCLUDE_SUDOKU_GENERATOR_H
