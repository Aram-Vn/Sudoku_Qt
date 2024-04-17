#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(100, 100, 700, 900);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setGeometry(0, 200, 700, 700);
    gridLayout = new QGridLayout(centralWidget);

    game = new Game(this);
    handleStart();
}

void MainWindow::handleStart()
{
    QVector<QVector<int>> board    = game->getBoard();
    const int             gridSize = 9;

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            QPushButton* button = new QPushButton();
            button->setFixedSize(60, 60);
            button->setStyleSheet("QPushButton {"
                                  "background-color: black;"
                                  "color: white;"
                                  "border-radius: 8px;"
                                  "font-size: 16px;"   
                                  "font-weight: bold;" 
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: white;"
                                  "border: 2px solid black;" 
                                  "color: black;"
                                  "border-radius: 10px;"
                                  "}");

            if (board[row][col])
            {
                button->setText(QString::number(board[row][col]));
            }
            gridLayout->addWidget(button, row, col);
        }
    }
}

MainWindow::~MainWindow() {}
