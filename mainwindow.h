#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event) override;

private:
    void handleStart();
    void addOnGrid();
    void changeHeartLabel();
    void resetGame();

private:
    Game*                 m_game;
    QGridLayout*          m_grid_layout;
    QVector<QPushButton*> m_difficulty_buttons;
    QPushButton*          m_start_button;
    QPushButton*          m_reset_game;
    QLabel*               m_heart_label;
    QLabel*               m_time_label;
    int                   m_seconds;
    QTimer*               m_timer;
};

#endif // MAINWINDOW_H
