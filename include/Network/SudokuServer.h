#ifndef SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_SERVER_H
#define SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_SERVER_H

#include <QDataStream>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class SudokuServer : public QObject
{
    Q_OBJECT

public:
    explicit SudokuServer(QObject* parent = nullptr);
    ~SudokuServer();

private slots:
    void sendGameData();

private:
    QTcpServer server;
};

#endif // SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_SERVER_H