// UIInitializer.cpp
#include "../include/UIInitializer.h"
#include "../include/SudokuButton/SudokuButton.h"
#include "../include/utils/color_utils.h"

UIInitializer::UIInitializer(QWidget* parent)
    : QObject(parent),
      m_parent(static_cast<QWidget*>(parent))
{
}

void UIInitializer::initializeUI()
{
    m_parent->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_parent->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                            "stop:0 #484848, stop:0.33 #383838, "
                            "stop:0.66 #333333, stop:1 #282828);");

    createLayout();
    createButtons();
    createLabels();
}

void UIInitializer::createLayout()
{
    m_central_widget = new QWidget(m_parent);
    m_central_widget->setGeometry(0, 150, WINDOW_WIDTH, WINDOW_WIDTH);
    m_grid_layout = new QGridLayout(m_central_widget);

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            createSudokuButton(row, col);
        }
    }
}

void UIInitializer::createSudokuButton(int row, int col)
{
    SudokuButton* sudokuButton = new SudokuButton(m_central_widget);
    sudokuButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);

    bool   isDark     = ((row / 3) % 2 == (col / 3) % 2);
    QColor baseColor  = isDark ? QColor("#018AAF") : QColor("#4FD9FF");
    QColor hoverColor = baseColor.darker(150);
    QColor focusColor = hoverColor.darker(150);

    QString colorStyle = colorUtil::colorStyleSet(baseColor, hoverColor, focusColor);
    sudokuButton->setStyleSheet(colorStyle);
    sudokuButton->setEnabled(false);

    m_grid_layout->addWidget(sudokuButton, row, col);
}

void UIInitializer::createButtons()
{
    QStringList difficulties = { "Easy", "Medium", "Hard" };
    for (int i = 0; i < 3; ++i)
    {
        QPushButton* button = new QPushButton(difficulties[i], m_parent);
        button->setGeometry(70 * (i + 1), 50, 70, 50);
        button->setStyleSheet("background-color: dimGray;");
        m_difficulty_buttons.push_back(button);
    }

    m_start_button = new QPushButton("Start", m_parent);
    m_start_button->setGeometry(280, 50, 70, 50);
    m_start_button->setStyleSheet("background-color: dimGray;");
    m_start_button->setEnabled(false);

    m_reset_game = new QPushButton("Reset", m_parent);
    m_reset_game->setGeometry(400, 50, 70, 50);
    m_reset_game->setStyleSheet("background-color: dimGray;");

    m_color_picker_button = new QPushButton("Choose Color", m_parent);
    m_color_picker_button->setGeometry(0, 0, 120, 45);
    m_color_picker_button->setStyleSheet("background-color: dimGray;");

    int buttonWidth  = 150;
    int buttonHeight = 50;
    int x            = (WINDOW_WIDTH - buttonWidth) / 2;
    int y            = (WINDOW_HEIGHT - buttonHeight) / 2;

    m_continue_old_game = new QPushButton("Continue old game", m_parent);
    m_continue_old_game->setStyleSheet("background-color: dimGray; text-align: center;");
    m_continue_old_game->setGeometry(x, y - 15, buttonWidth, buttonHeight);
}

void UIInitializer::createLabels()
{
    m_heart_label = new QLabel(m_parent);
    m_heart_label->setGeometry(500, 50, 90, 50);
    m_heart_label->setAlignment(Qt::AlignCenter);
    m_heart_label->setStyleSheet("background-color: dimGray; border-radius: 25px;");
    m_heart_label->setText("HeartCount");

    m_time_label = new QLabel(m_parent);
    m_time_label->setText("00:00:00");
    m_time_label->setGeometry(600, 50, 70, 50);
    m_time_label->setAlignment(Qt::AlignCenter);
    m_time_label->setStyleSheet("background-color: dimGray;");
}