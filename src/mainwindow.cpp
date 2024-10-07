#include "../include/mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_game(new Game(this)),
      m_difficulty_buttons(3),
      m_continue_old_game(3),
      m_start_button(new QPushButton(this)),
      m_reset_game(new QPushButton(this)),
      m_heart_label(new QLabel(this)),
      m_time_label(new QLabel(this)),
      m_timer(new QTimer(this)),
      m_seconds{},
      m_is_left_click{ true },
      m_color_picker_button(new QPushButton(this)),
      m_start(new QPushButton(this))
{
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_central_widget = new QWidget(this);
    m_central_widget->setGeometry(0, 150, 700, 700);

    this->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::MAIN));

    m_grid_layout = new QGridLayout(m_central_widget);

    const int grid_size = GRID_SIZE;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            SudokuButton* sudokuButton = new SudokuButton(this);
            sudokuButton->setFixedSize(SUDOKU_BUTTON_SIZE, SUDOKU_BUTTON_SIZE);

            bool   is_dark   = ((row / 3) % 2 == (col / 3) % 2);
            QColor baseColor = is_dark ? QColor("#0c438f") : QColor("#a2a3a0");

            QString colorStyle = colorUtil::colorStyleSet(baseColor);

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
        m_difficulty_buttons[i]->hide();
        m_difficulty_buttons[i]->setGeometry(86 * i + 5, 50, 85, BUTTON_HEIGHT);
        m_difficulty_buttons[i]->setStyleSheet(colorUtil::getStyle(static_cast<colorUtil::buttonType>(i)));

        connect(m_difficulty_buttons[i], &QPushButton::clicked, this,
                [this, i]()
                {
                    m_game->setDifficulty(i);
                    m_start_button->setEnabled(true);
                });
    }

    int buttonWidth = 300;

    int x = (this->width() - buttonWidth) / 2;
    int y = (this->height() - BUTTON_HEIGHT) / 2 - 100;

    QStringList continue_txt = { "continue save 1", "continue save 2", "continue save 3" };

    for (int i = 0; i < m_continue_old_game.size(); ++i)
    {
        m_continue_old_game[i] = new QPushButton(continue_txt[i], this);
        m_continue_old_game[i]->setGeometry(x, y + 70 * i, buttonWidth, BUTTON_HEIGHT);
        m_continue_old_game[i]->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::CUSTOM));

        connect(m_continue_old_game[i], &QPushButton::clicked, this, [this, i]() { promptContinueOldGame(i); });
    }

    m_start_button->setText("Start");
    m_start_button->setGeometry(300, 50, 70, BUTTON_HEIGHT);
    m_start_button->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::CUSTOM));
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
    m_reset_game->setGeometry(375, 50, 70, BUTTON_HEIGHT);
    m_reset_game->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::CUSTOM));

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

    m_heart_label->setGeometry(480, 50, 90, BUTTON_HEIGHT);
    m_heart_label->setAlignment(Qt::AlignCenter);
    m_heart_label->setStyleSheet("background-color: dimGray; border-radius: 25px;");

    QString heartSpan = colorUtil::generateHeartSpan();
    m_heart_label->setText(heartSpan);
    m_heart_label->setTextFormat(Qt::RichText);

    m_time_label->setText("00:00:00");
    m_time_label->setGeometry(580, 50, 100, BUTTON_HEIGHT);
    m_time_label->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::LABEL));
    m_time_label->setAlignment(Qt::AlignCenter);

    m_color_picker_button->setText("Choose Color");
    m_color_picker_button->setGeometry(0, 0, 150, 45);
    m_color_picker_button->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::CUSTOM));
    connect(m_color_picker_button, &QPushButton::clicked, this, &MainWindow::openColorPicker);

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

    m_reset_game->hide();
    m_heart_label->hide();
    m_start_button->hide();
    m_time_label->hide();
    m_color_picker_button->hide();
    m_central_widget->hide();

    m_start->setGeometry(x, y - 70, buttonWidth, BUTTON_HEIGHT);
    m_start->setText("New game");
    m_start->setStyleSheet(colorUtil::getStyle(colorUtil::buttonType::CUSTOM));
    connect(m_start, &QPushButton::clicked, this, &MainWindow::showHidden);
}

MainWindow::~MainWindow() {}

void MainWindow::showHidden()
{
    for (auto& button : m_difficulty_buttons)
    {
        button->show();
    }

    m_reset_game->show();
    m_heart_label->show();
    m_start_button->show();
    m_time_label->show();
    m_color_picker_button->show();
    m_central_widget->show();

    for (int i = 0; i < m_continue_old_game.size(); ++i)
    {
        delete m_continue_old_game[i];
    }

    delete m_start;
}

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
    int     heartCount = m_game->getHearts();
    QString heartSpan  = colorUtil::generateHeartSpan(heartCount);

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

    int     heartCount = m_game->getHearts();
    QString heartSpan  = colorUtil::generateHeartSpan(heartCount);

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

    m_game->setDifficulty(-1);
    m_game->setHearts(-1);
    m_game->setEmptyCount(-1);

    for (int i = 0; i < m_difficulty_buttons.size(); ++i)
    {
        m_difficulty_buttons[i]->setEnabled(true);
    }

    m_seconds = 0;
    m_timer->stop();

    QString heartSpan = colorUtil::generateHeartSpan();

    m_heart_label->setText(heartSpan);
    m_heart_label->setTextFormat(Qt::RichText);
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

            QString colorStyle = colorUtil::colorStyleSet(baseColor);
            sudokuButton->setStyleSheet(colorStyle);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    GameStateManager::saveGameState(m_game, m_grid_layout, m_seconds);
    event->accept();
}

void MainWindow::promptContinueOldGame(int i)
{
    qDebug() << i;
    QMessageBox msgBox;
    msgBox.setWindowTitle("Continue Game");
    msgBox.setText("Do you want to continue your old game?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    msgBox.setStyleSheet("QLabel{color: white;} "
                         "QMessageBox{background-color: #22262e;}");

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes)
    {
        bool isSuccess = GameStateManager::loadGameState(m_game, m_grid_layout, m_seconds);
        if (!isSuccess)
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

        this->showHidden();
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