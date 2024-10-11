#include "../../include/Network/SudokuServer.h"

SudokuServer::SudokuServer(QObject* parent)
    : QObject(parent)
{
    connect(&server, &QTcpServer::newConnection, this, &SudokuServer::sendGameData);
    if (!server.listen(QHostAddress::Any, 12345))
    {
        qWarning() << "Server could not start!";
    }
    else
    {
        qInfo() << "Server started on port 12345";
    }
}

SudokuServer::~SudokuServer()
{
    qInfo() << "Server closed";

    server.close();
}

void SudokuServer::sendGameData()
{
    QTcpSocket* clientSocket = server.nextPendingConnection();
    if (clientSocket)
    {
        QVector<QVector<int>> board;
        QVector<QVector<int>> fullBoard;
        int                   difficulty = 1;
        int                   emptyCount = 20;

        QDataStream out(clientSocket);
        out.setVersion(QDataStream::Qt_5_15);

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                out << (quint8)board[row][col];
            }
        }

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                out << (quint8)fullBoard[row][col];
            }
        }

        out << (quint8)difficulty;
        out << (quint8)emptyCount;

        clientSocket->disconnectFromHost();
    }
}