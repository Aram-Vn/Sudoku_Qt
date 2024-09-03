#ifndef GAME_H
#define GAME_H

#include "sudokugenerator.h"
#include <QObject>
#include <QVector>
#include <QWidget>
#include <cstdlib>
#include <ctime>

class Game : public QObject
{
    Q_OBJECT
public:
    Game(QObject* parent = nullptr);

signals:
    void board_is_ready();
    void add_on_grid();
    void change_hearts_count();

private:
    void init_block(int i, int j);

public:
    QVector<QVector<int>> getFullBoard() const;
    void                  setFullBoard(const QVector<QVector<int>>& boar);

    void                  setBoard(const QVector<QVector<int>>& board);
    QVector<QVector<int>> getBoard() const;

    void initGame();
    void setCoords(int i, int j);
    void numberEvent(int number);
    int  getX() const;
    int  getY() const;
    int  getNumber(int i, int j) const;

    void setHearts(const int count);
    int  getHearts() const;

    int  getEmptyCount() const;
    void setEmptyCount(int count);

    void setDifficulty(int diff);
    int  getDifficulty() const;

    bool checkPos(int i, int j) const;

private:
    QVector<QVector<int>> m_full_board;
    QVector<QVector<int>> m_board;
    int                   m_difficulty;
    int                   m_x;
    int                   m_y;
    int                   m_hearts;
    int                   m_empty_fields;
};

#endif // GAME_H
