#include "../../include/Network/SudokuClient.h"

SudokuClient::SudokuClient(const QString& serverIp, quint16 serverPort, QObject* parent)
    : QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, this, &SudokuClient::onConnected);
    connect(&socket, &QTcpSocket::disconnected, this, &SudokuClient::onDisconnected);
    
    socket.connectToHost(serverIp, serverPort); 
}

void SudokuClient::onConnected()
{
    qDebug() << "Connected to server at" << socket.peerAddress().toString() << "on port" << socket.peerPort();
    receiveGameData();
}

void SudokuClient::onDisconnected()
{
    qDebug() << "Disconnected from server.";
}

void SudokuClient::receiveGameData()
{
    if (socket.waitForReadyRead(3000))
    {
        QDataStream in(&socket);
        in.setVersion(QDataStream::Qt_5_15);

        QString data;
        in >> data;
        qDebug() << "Received message:" << data;

        qDebug() << "hi";
    }
    else
    {
        qDebug() << "No data received within timeout.";
    }

    // socket.disconnectFromHost();
}

SudokuClient::~SudokuClient()
{
    if (socket.isOpen())
    {
        socket.disconnectFromHost();
    }
}
