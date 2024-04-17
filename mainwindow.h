#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void handleStart();

private:
    Game*        game;
    QGridLayout* gridLayout;
    QLabel*      hor_lbl1;
    QLabel*      hor_lbl2;
    QLabel*      ver_lbl1;
    QLabel*      ver_lbl2;
};

#endif // MAINWINDOW_H
