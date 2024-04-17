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
    Game(QObject *parent = nullptr);

public:
    QVector<QVector<int>> getBoard() const;

private:
    void init_block(int i, int j);

private:
    QVector<QVector<int>> full_board;
    QVector<QVector<int>> board;
    int                   difficulty;
};

#endif // GAME_H
