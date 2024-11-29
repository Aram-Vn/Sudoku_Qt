#ifndef SUDOKU_QT_INCLUDE_MAIN_WINDOW_H
#define SUDOKU_QT_INCLUDE_MAIN_WINDOW_H

#include "../include/GameStateManager.h"
#include "../include/SudokuButton/SudokuButton.h"
#include "../include/game.h"
#include "../include/utils/color_utils.h"
#include "../include/utils/file_utils.h"

#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
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
#include <QPixmap>
#include <QPushButton>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void openColorPicker();
    void promptContinueOldGame(int index);
    void showHidden();

private:
    void handleStart();
    void addOnGrid();
    void changeHeartLabel();
    void resetGame();

    void setUi();
    void setConnections();

private:
    static constexpr int WINDOW_WIDTH       = 700;
    static constexpr int WINDOW_HEIGHT      = 850;
    static constexpr int GRID_SIZE          = 9;
    static constexpr int SUDOKU_BUTTON_SIZE = 60;
    static constexpr int BUTTON_HEIGHT      = 50;
    static constexpr int BUTTON_WIDTH       = 300;

private:
    Game*                 m_game;
    QVector<QPushButton*> m_difficulty_buttons;
    QPushButton*          m_start_button;
    QVector<QPushButton*> m_continue_old_game;
    QPushButton*          m_reset_game;
    QLabel*               m_heart_label;
    QLabel*               m_time_label;
    quint32               m_seconds;
    QTimer*               m_timer;
    bool                  m_is_left_click;
    QPushButton*          m_color_picker_button;
    QWidget*              m_central_widget;
    QGridLayout*          m_grid_layout;
    savefile              m_fileIND;
};

#endif // SUDOKU_QT_INCLUDE_MAIN_WINDOW_H
