#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <vector>

class SudokuButton;

class UIInitializer : public QObject
{
    Q_OBJECT

public:
    explicit UIInitializer(QWidget* parent);

    void initializeUI();

    QWidget* getCentralWidget() const
    {
        return m_central_widget;
    }
    QGridLayout* getGridLayout() const
    {
        return m_grid_layout;
    }
    const std::vector<QPushButton*>& getDifficultyButtons() const
    {
        return m_difficulty_buttons;
    }
    QPushButton* getStartButton() const
    {
        return m_start_button;
    }
    QPushButton* getContinueOldGameButton() const
    {
        return m_continue_old_game;
    }
    QPushButton* getResetGameButton() const
    {
        return m_reset_game;
    }
    QLabel* getHeartLabel() const
    {
        return m_heart_label;
    }
    QLabel* getTimeLabel() const
    {
        return m_time_label;
    }
    QPushButton* getColorPickerButton() const
    {
        return m_color_picker_button;
    }

private:
    void createLayout();
    void createButtons();
    void createLabels();
    void createSudokuButton(int row, int col);

    QWidget*                  m_parent;
    QWidget*                  m_central_widget;
    QGridLayout*              m_grid_layout;
    std::vector<QPushButton*> m_difficulty_buttons;
    QPushButton*              m_start_button;
    QPushButton*              m_continue_old_game;
    QPushButton*              m_reset_game;
    QLabel*                   m_heart_label;
    QLabel*                   m_time_label;
    QPushButton*              m_color_picker_button;

    static constexpr int WINDOW_WIDTH  = 700;
    static constexpr int WINDOW_HEIGHT = 850;
    static constexpr int GRID_SIZE     = 9;
    static constexpr int BUTTON_SIZE   = 60;
};
