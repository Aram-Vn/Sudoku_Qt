#include "../include/mainwindow.h"
#include "utils/file_utils.h"
#include <qpushbutton.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_game(new Game(this)),
      m_difficulty_buttons(3),
      m_start_button(new QPushButton(this)),
      m_continue_old_game(new QPushButton(this)),
      m_reset_game(new QPushButton(this)),
      m_heart_label(new QLabel(this)),
      m_time_label(new QLabel(this)),
      m_timer(new QTimer(this)),
      m_seconds{},
      m_is_left_click{ true },
      m_colorPickerButton(new QPushButton(this))
{
    this->setFixedSize(700, 850);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setGeometry(0, 150, 700, 700);
    this->setStyleSheet("background-color: #282828;");
    m_grid_layout = new QGridLayout(centralWidget);

    const int grid_size = 9;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            SudokuButton* sudokuButton = new SudokuButton(this);
            sudokuButton->setFixedSize(60, 60);

            bool is_dark = ((row / 3) % 2 == (col / 3) % 2);

            QColor baseColor  = is_dark ? QColor("#018AAF") : QColor("#4FD9FF");
            QColor hoverColor = baseColor.darker(150);
            QColor focusColor = hoverColor.darker(150);

            QString colorStyle = colorUtil::colorStyleSet(baseColor, hoverColor, focusColor);

            sudokuButton->setStyleSheet(colorStyle);
            sudokuButton->setEnabled(false);

            connect(sudokuButton, &SudokuButton::leftClicked, this,
                    [row, col, this]()
                    {
                        m_is_left_click = true;

                        int x = m_game->getX();
                        int y = m_game->getY();

                        if (m_game->checkPos(row, col))
                        {
                            m_game->setCoords(row, col);
                        }
                    });

            connect(sudokuButton, &SudokuButton::rightClicked, this, [this]() { m_is_left_click = false; });

            m_grid_layout->addWidget(sudokuButton, row, col);
        }
    }

    QStringList difficulties = { "Easy", "Medium", "Hard" };

    for (int i = 0; i < m_difficulty_buttons.size(); ++i)
    {
        m_difficulty_buttons[i] = new QPushButton(difficulties[i], this);
        m_difficulty_buttons[i]->setGeometry(70 * (i + 1), 50, 70, 50);
        m_difficulty_buttons[i]->setStyleSheet("background-color: dimGray;");
        connect(m_difficulty_buttons[i], &QPushButton::clicked, this,
                [this, i]()
                {
                    m_game->setDifficulty(i);
                    m_start_button->setEnabled(true);
                });
    }

    m_start_button->setText("Start");
    m_start_button->setGeometry(280, 50, 70, 50);
    m_start_button->setStyleSheet("background-color: dimGray;");
    m_start_button->setEnabled(false);

    connect(m_start_button, &QPushButton::clicked, this,
            [this]()
            {
                m_game->initGame();
                for (int i = 0; i < m_difficulty_buttons.size(); ++i)
                {
                    m_difficulty_buttons[i]->setEnabled(false);
                }

                m_start_button->setEnabled(false);
            });

    m_reset_game->setText("Reset");
    m_reset_game->setGeometry(400, 50, 70, 50);
    m_reset_game->setStyleSheet("background-color: dimGray;");

    connect(
        m_reset_game, &QPushButton::clicked, this,
        [this]()
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Reset The Game??");
            msgBox.setText("Do you want to Reset your game?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            int reply = msgBox.exec();

            if (reply == QMessageBox::Yes)
            {
                for (int row = 0; row < grid_size; ++row)
                {
                    for (int col = 0; col < grid_size; ++col)
                    {
                        dynamic_cast<SudokuButton*>(m_grid_layout->itemAtPosition(row, col)->widget())->clearNumber();
                    }
                }

                m_time_label->setText("00:00:00");
                resetGame();
                QMessageBox::information(nullptr, "Reset", "you give up!!!");
            }
            return;
        });

    m_heart_label->setGeometry(500, 50, 90, 50);
    m_heart_label->setAlignment(Qt::AlignCenter);
    m_heart_label->setStyleSheet("background-color: dimGray; border-radius: 25px;");
    m_heart_label->setText("Heartcount");

    m_time_label->setText("00:00:00");
    m_time_label->setGeometry(600, 50, 70, 50);
    m_time_label->setAlignment(Qt::AlignCenter);
    m_time_label->setStyleSheet("background-color: dimGray;");

    m_colorPickerButton->setText("Choose Color");
    m_colorPickerButton->setGeometry(70, 100, 150, 50);
    m_colorPickerButton->setStyleSheet("background-color: dimGray;");
    connect(m_colorPickerButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);

    connect(m_game, &Game::board_is_ready, this, &MainWindow::handleStart);
    connect(m_game, &Game::add_on_grid, this, &MainWindow::addOnGrid);
    connect(m_game, &Game::change_hearts_count, this, &MainWindow::changeHeartLabel);

    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout, this,
            [this]()
            {
                ++m_seconds;
                int hours   = m_seconds / 3600;
                int minutes = (m_seconds % 3600) / 60;
                int secs    = m_seconds % 60;
                m_time_label->setText(QString("%1:%2:%3")
                                          .arg(hours, 2, 10, QLatin1Char('0'))
                                          .arg(minutes, 2, 10, QLatin1Char('0'))
                                          .arg(secs, 2, 10, QLatin1Char('0')));
            });

    m_continue_old_game->setText("Continue old game");
    m_continue_old_game->setStyleSheet("background-color: dimGray;"
                                       "text-align: left;"
                                       "padding-left: 8px;");
    m_continue_old_game->setGeometry(0, 0, 145, 40);

    connect(m_continue_old_game, &QPushButton::clicked, this, [this]() { promptContinueOldGame(); });
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (m_is_left_click)
    {
        if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9)
        {
            int number = event->key() - Qt::Key_0;
            m_game->numberEvent(number);
        }
    }
}

void MainWindow::handleStart()
{
    int     hearts = m_game->getHearts();
    QString heartSpan;
    for (int i = 0; i < hearts; ++i)
    {
        heartSpan += "<img src=':/assets/Heart.png' width='17' height='17'>";
    }
    
    m_heart_label->setText(heartSpan);
    m_heart_label->setTextFormat(Qt::RichText);

    QVector<QVector<int>> board     = m_game->getBoard();
    const int             grid_size = 9;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            SudokuButton* button = dynamic_cast<SudokuButton*>(m_grid_layout->itemAtPosition(row, col)->widget());

            if (board[row][col])
            {
                button->setText(QString::number(board[row][col]));
                button->setEnabled(false);
            }
            else
            {
                button->setText("");
                button->setEnabled(true);
            }
        }
    }

    m_timer->start();
}

void MainWindow::addOnGrid()
{
    int x = m_game->getX();
    int y = m_game->getY();

    SudokuButton* button = dynamic_cast<SudokuButton*>(m_grid_layout->itemAtPosition(x, y)->widget());

    button->setText(QString::number(m_game->getNumber(x, y)));
    button->setEnabled(false);
    button->clearNumber();

    m_game->setCoords(-1, -1);

    if (m_game->getEmptyCount() == 0)
    {
        m_timer->stop();
        QMessageBox::information(nullptr, "Win", QString("You won!!!\nYour time is: %1").arg(m_time_label->text()));
    }
}

void MainWindow::changeHeartLabel()
{
    QMessageBox::information(nullptr, "wrong", "No!!!");

    int     hearts = m_game->getHearts();
    QString heartSpan;
    for (int i = 0; i < hearts; ++i)
    {
        heartSpan += "<img src=':/assets/Heart.png' width='17' height='17'>";
    }

    m_heart_label->setText(heartSpan);
    m_heart_label->setTextFormat(Qt::RichText);

    if (m_game->getHearts() == 0)
    {
        m_timer->stop();
        resetGame();
        QMessageBox::information(nullptr, "wrong", "looser");
    }
}

void MainWindow::resetGame()
{
    const int grid_size = 9;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            SudokuButton* button = dynamic_cast<SudokuButton*>(m_grid_layout->itemAtPosition(row, col)->widget());
            button->setText("");
            button->setEnabled(false);
            button->clearNumber();
        }
    }

    if (m_game->getX() != -1)
    {
        m_game->setCoords(-1, -1);
    }

    for (int i = 0; i < m_difficulty_buttons.size(); ++i)
    {
        m_difficulty_buttons[i]->setEnabled(true);
    }

    m_seconds = 0;
    m_timer->stop();
    m_heart_label->setText("Hearts:");
}

void MainWindow::openColorPicker()
{
    QColor colorDark = QColorDialog::getColor(Qt::white, this, "Choose Dark Button Color");
    if (!colorDark.isValid())
    {
        return;
    }

    QColor colorLight = QColorDialog::getColor(Qt::black, this, "Choose Light Button Color");
    if (!colorLight.isValid())
    {
        return;
    }

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            QPushButton* sudokuButton = dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget());

            bool   is_dark   = ((row / 3) % 2 == (col / 3) % 2);
            QColor baseColor = is_dark ? colorDark : colorLight;

            QColor  hoverColor{};
            QColor  focusColor{};
            QString FocusTextColor{};
            QString textColor{};

            if (baseColor == Qt::black || baseColor.lightness() < 10)
            {
                // If the base color is black, set hover and focus colors to dark gray
                hoverColor     = QColor(50, 50, 50);    // Dark gray for hover
                focusColor     = QColor(100, 100, 100); // Lighter gray for focus
                textColor      = "white";
                FocusTextColor = "black";
            }
            else if (baseColor.lightness() < 128)
            {
                // Make hover and focus colors lighter if base color is dark
                hoverColor     = baseColor.lighter(120);
                focusColor     = hoverColor.lighter(120);
                textColor      = "white";
                FocusTextColor = "black";
            }
            else
            {
                // Make hover and focus colors darker if base color is light
                hoverColor     = baseColor.darker(150);
                focusColor     = hoverColor.darker(150);
                textColor      = "black";
                FocusTextColor = "white";
            }

            QString colorStyle = colorUtil::colorStyleSet(baseColor, hoverColor, focusColor, textColor, FocusTextColor);

            sudokuButton->setStyleSheet(colorStyle);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    saveGameState();
    event->accept();
}

void MainWindow::saveGameState()
{
    QString filePath = "sudoku_save.bin";

    QPushButton* darkStyleButton = dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(0, 0)->widget());
    QString      darkStyle       = darkStyleButton->styleSheet();

    QPushButton* lightStyleButton = dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(0, 4)->widget());
    QString      lightStyle       = lightStyleButton->styleSheet();

    QVector<QVector<int>> board     = m_game->getBoard();
    QVector<QVector<int>> fullBoard = m_game->getFullBoard();

    QVector<QVector<QString>> TopRightButtonNumbers(9, QVector<QString>(9, QString()));
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            SudokuButton* button = dynamic_cast<SudokuButton*>(m_grid_layout->itemAtPosition(row, col)->widget());
            if (button)
            {
                TopRightButtonNumbers[row][col] = button->getTopRightNumber(); // Get number for each button
            }
        }
    }

    fileUtil::writeInBinary(filePath, board, fullBoard, m_game->getDifficulty(), m_game->getEmptyCount(),
                            m_game->getHearts(), m_seconds, darkStyle, lightStyle, TopRightButtonNumbers);
}

bool MainWindow::loadGameState()
{
    QString                   filePath = "sudoku_save.bin";
    QVector<QVector<int>>     board{};
    QVector<QVector<int>>     fullBoard{};
    int                       difficulty{};
    int                       emptyCount{};
    int                       heartCount{};
    int                       seconds{};
    QString                   darkStyle{};
    QString                   lightStyle{};
    QVector<QVector<QString>> TopRightButtonNumbers{};

    bool is_success = fileUtil::readFromBinary(filePath, board, fullBoard, difficulty, emptyCount, heartCount, seconds,
                                               darkStyle, lightStyle, TopRightButtonNumbers);

    if (is_success)
    {
        m_game->setBoard(board);
        m_game->setFullBoard(fullBoard);
        m_game->setDifficulty(difficulty);
        m_game->setEmptyCount(emptyCount);
        m_game->setHearts(heartCount);
        m_seconds = seconds;

        colorUtil::applyColorStyles(m_grid_layout, darkStyle, lightStyle);

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                SudokuButton* button = dynamic_cast<SudokuButton*>(m_grid_layout->itemAtPosition(row, col)->widget());
                if (button)
                {
                    button->setTopRightNumber(TopRightButtonNumbers[row][col]);
                }
            }
        }

        return true;
    }

    return false;
}

void MainWindow::promptContinueOldGame()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Continue Game");
    msgBox.setText("Do you want to continue your old game?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    msgBox.setStyleSheet("QLabel{color: white;} "
                         "QMessageBox{background-color: #22262e;}");

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes)
    {
        if (!loadGameState())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Attention");
            msgBox.setText("There is no old valid game to continue");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet("QLabel{color: white;} "
                                 "QMessageBox{background-color: #22262e;}");
            msgBox.exec();
            return;
        }

        this->handleStart();

        for (int i = 0; i < m_difficulty_buttons.size(); ++i)
        {
            m_difficulty_buttons[i]->setEnabled(false);
        }
    }
    else
    {
        this->resetGame();
    }
}