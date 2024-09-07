#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/game.h"
#include "../include/utils/color_utils.h"
#include "../include/utils/file_utils.h"

#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event) override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void openColorPicker();
    void promptContinueOldGame();

private:
    void handleStart();
    void addOnGrid();
    void changeHeartLabel();
    void resetGame();
    //
    void saveGameState();
    bool loadGameState();

private:
    Game*                 m_game;
    QGridLayout*          m_grid_layout;
    QVector<QPushButton*> m_difficulty_buttons;
    QPushButton*          m_start_button;
    QPushButton*          m_continue_old_game;
    QPushButton*          m_reset_game;
    QLabel*               m_heart_label;
    QLabel*               m_time_label;
    int                   m_seconds;
    QTimer*               m_timer;
};

#endif // MAINWINDOW_H
