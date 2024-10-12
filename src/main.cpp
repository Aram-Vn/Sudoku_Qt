#include "../Network/include/SudokuClient.h"
#include "../Network/include/SudokuServer.h"
#include "../include/mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    SudokuServer::instance().startServer(12345); // test
    SudokuClient c{ "127.0.0.1", 12345 };        // test

    MainWindow w;
    w.setWindowIcon(QIcon(":assets/Sudoku_Qt.ico"));
    w.setWindowTitle("Sudoku_Qt");
    w.show();
    return a.exec();
}
