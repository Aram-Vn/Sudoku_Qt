#ifndef SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_SERVER_H
#define SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_SERVER_H

#include <QDataStream>
#include <QDebug>
#include <QHostAddress>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class SudokuServer : public QObject
{
    Q_OBJECT

public:
    static SudokuServer& instance()
    {
        static SudokuServer instance;
        return instance;
    }

    void startServer(quint16 port);

private slots:
    void sendGameData();

private:
    explicit SudokuServer(QObject* parent = nullptr);
    ~SudokuServer()
    {
        // ....
    }

    QTcpServer server;
};

#endif // SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_SERVER_H
