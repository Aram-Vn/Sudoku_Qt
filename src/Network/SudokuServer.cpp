#include "../../include/Network/SudokuServer.h"

SudokuServer::SudokuServer(QObject* parent)
    : QObject(parent)
{
}

void SudokuServer::startServer(quint16 port)
{
    if (!server.listen(QHostAddress::Any, port))
    {
        qDebug() << "Server could not start!";
        return;
    }
    qDebug() << "Server started on port" << port;

    connect(&server, &QTcpServer::newConnection, this, &SudokuServer::sendGameData);
}

void SudokuServer::sendGameData()
{
    QTcpSocket* clientSocket = server.nextPendingConnection();
    if (clientSocket)
    {
        QDataStream out(clientSocket);
        out.setVersion(QDataStream::Qt_5_15);

        QString message = "Hi";
        out << message;

        clientSocket->flush();
        clientSocket->waitForBytesWritten(3000);
        qDebug() << "Sent message:" << message;

        // clientSocket->disconnectFromHost();
    }
}
