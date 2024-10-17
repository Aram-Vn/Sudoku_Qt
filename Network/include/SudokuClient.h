#ifndef SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_CLIENT_H
#define SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_CLIENT_H

#include <QDataStream>
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QVector>

class SudokuClient : public QObject
{
    Q_OBJECT

public:
    explicit SudokuClient(const QString& serverIp, quint16 serverPort, QObject* parent = nullptr);
    ~SudokuClient();

private slots:
    void onConnected();
    void onDisconnected();
    void receiveGameData();

private:
    QTcpSocket socket;
};

#endif // SUDOKU_QT_INCLUDE_NETWORK_SUDOKU_CLIENT_H
