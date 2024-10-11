#include "../include/Network/SudokuServer.h"
#include "../include/mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    SudokuServer s;

    MainWindow w;
    w.setWindowIcon(QIcon(":assets/Sudoku_Qt.ico"));
    w.setWindowTitle("Sudoku_Qt");
    w.show();
    return a.exec();
}
