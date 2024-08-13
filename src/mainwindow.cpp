#include "../include/mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_difficulty_buttons(3),
      m_start_button(new QPushButton(this)),
      m_reset_game(new QPushButton(this)),
      m_heart_label(new QLabel(this)),
      m_time_label(new QLabel(this)),
      m_timer(new QTimer(this))

{
    this->setGeometry(100, 100, 700, 900);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setGeometry(0, 150, 700, 700);
    this->setStyleSheet("background-color: #282828;");
    m_grid_layout = new QGridLayout(centralWidget);

    const int grid_size = 9;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            QPushButton* sudokuButton = new QPushButton();
            sudokuButton->setFixedSize(60, 60);

            bool is_dark = ((row / 3) % 2 == (col / 3) % 2);

            QColor baseColor  = is_dark ? QColor("#018AAF") : QColor("#4FD9FF");
            QColor hoverColor = baseColor.darker(150);

            QString colorStyle = QString("QPushButton {"
                                         "background-color: %1;"
                                         "color: black;"
                                         "border-radius: 8px;"
                                         "font-size: 16px;"
                                         "font-weight: bold;"
                                         "}"
                                         "QPushButton:hover {"
                                         "background-color: %2;"
                                         "border: 2px solid black;"
                                         "color: white;"
                                         "border-radius: 10px;"
                                         "}")
                                     .arg(baseColor.name())
                                     .arg(hoverColor.name());

            sudokuButton->setStyleSheet(colorStyle);

            sudokuButton->setEnabled(false);

            connect(sudokuButton, &QPushButton::clicked, this,
                    [row, col, this]()
                    {
                        int x = m_game->getX();
                        int y = m_game->getY();

                        if (m_game->checkPos(row, col))
                        {
                            m_game->setCoords(row, col);
                        }
                    });

            m_grid_layout->addWidget(sudokuButton, row, col);
        }
    }

    m_game = new Game(this);

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
    connect(m_reset_game, &QPushButton::clicked, this,
            [this]()
            {
                m_time_label->setText("00:00:00");
                resetGame();
                QMessageBox::information(nullptr, "Reset", "you give up!!!");
            });

    m_heart_label->setGeometry(500, 50, 90, 50);
    m_heart_label->setAlignment(Qt::AlignCenter);
    m_heart_label->setStyleSheet("background-color: dimGray;");
    m_heart_label->setText("Hearts:");

    m_time_label->setText("00:00:00");
    m_time_label->setGeometry(600, 50, 70, 50);
    m_time_label->setAlignment(Qt::AlignCenter);
    m_time_label->setStyleSheet("background-color: dimGray;");

    QPushButton* colorPickerButton = new QPushButton("Choose Color", this);
    colorPickerButton->setGeometry(70, 100, 150, 50);
    colorPickerButton->setStyleSheet("background-color: dimGray;");
    connect(colorPickerButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);

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
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9)
    {
        int number = event->key() - Qt::Key_0;
        m_game->numberEvent(number);
    }
}

void MainWindow::handleStart()
{
    m_heart_label->setText("Hearts: <span style='color:red;'>♥&nbsp;♥&nbsp;♥</span>");
    m_time_label->setText("00:00:00");
    QVector<QVector<int>> board     = m_game->getBoard();
    const int             grid_size = 9;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            if (board[row][col])
            {
                dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget())
                    ->setText(QString::number(board[row][col]));
            }

            else
            {
                dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget())->setText("");
            }

            dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget())->setEnabled(true);
        }
    }

    m_timer->start();

    m_seconds = 0;
}

void MainWindow::addOnGrid()
{
    int x = m_game->getX();
    int y = m_game->getY();
    dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(x, y)->widget())
        ->setText(QString::number(m_game->getNumber(x, y)));

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

    QString heartString;
    for (int i = 0; i < m_game->getHearts(); ++i)
    {
        heartString.append("♥&nbsp;");
    }

    m_heart_label->setText("Hearts: <span style='color:red;'>" + heartString.trimmed() + "</span>");
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
            dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget())->setText("");
            dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget())->setEnabled(false);
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

    QColor colorLight = QColorDialog::getColor(Qt::white, this, "Choose Light Button Color");
    if (!colorLight.isValid())
    {
        return;
    }

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            QPushButton* sudokuButton = dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget());

            bool   is_dark    = ((row / 3) % 2 == (col / 3) % 2);
            QColor hoverColor = is_dark ? colorDark.darker(150) : colorLight.darker(150);

            QString colorStyle;
            if (is_dark)
            {
                colorStyle = QString("QPushButton {"
                                     "background-color: %1;"
                                     "color: black;"
                                     "border-radius: 8px;"
                                     "font-size: 16px;"
                                     "font-weight: bold;"
                                     "}"
                                     "QPushButton:hover {"
                                     "background-color: %2;"
                                     "border: 2px solid black;"
                                     "color: white;"
                                     "border-radius: 10px;"
                                     "}")
                                 .arg(colorDark.name())
                                 .arg(hoverColor.name());
            }
            else
            {
                colorStyle = QString("QPushButton {"
                                     "background-color: %1;"
                                     "color: black;"
                                     "border-radius: 8px;"
                                     "font-size: 16px;"
                                     "font-weight: bold;"
                                     "}"
                                     "QPushButton:hover {"
                                     "background-color: %2;"
                                     "border: 2px solid black;"
                                     "color: white;"
                                     "border-radius: 10px;"
                                     "}")
                                 .arg(colorLight.name())
                                 .arg(hoverColor.name());
            }

            sudokuButton->setStyleSheet(colorStyle);
        }
    }
}
