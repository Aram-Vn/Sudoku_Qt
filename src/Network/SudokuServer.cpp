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
