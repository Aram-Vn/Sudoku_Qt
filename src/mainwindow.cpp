#include "../include/mainwindow.h"
#include <qmessagebox.h>

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
    m_grid_layout = new QGridLayout(centralWidget);

    const int grid_size = 9;
    for (int row = 0; row < grid_size; ++row)
    {
        for (int col = 0; col < grid_size; ++col)
        {
            QPushButton* button = new QPushButton();
            button->setFixedSize(60, 60);

            bool is_dark = ((row / 3) % 2 == (col / 3) % 2);

            if (is_dark)
            {
                button->setStyleSheet("QPushButton {"
                                      "background-color: #015E77;"
                                      "color: black;"
                                      "border-radius: 8px;"
                                      "font-size: 16px;"
                                      "font-weight: bold;"
                                      "}"
                                      "QPushButton:hover {"
                                      "background-color: #0094B6;"
                                      "border: 2px solid black;"
                                      "color: white;"
                                      "border-radius: 10px;"
                                      "}");
            }
            else
            {
                button->setStyleSheet("QPushButton {"
                                      "background-color: #4FD9FF;"
                                      "color: black;"
                                      "border-radius: 8px;"
                                      "font-size: 16px;"
                                      "font-weight: bold;"
                                      "}"
                                      "QPushButton:hover {"
                                      "background-color: #0094B6;"
                                      "border: 2px solid black;"
                                      "color: white;"
                                      "border-radius: 10px;"
                                      "}");
                ;
            }

            button->setEnabled(false);

            connect(button, &QPushButton::clicked, this,
                    [row, col, this]()
                    {
                        int x = m_game->getX();
                        int y = m_game->getY();

                        if (m_game->checkPos(row, col))
                        {
                            m_game->setCoords(row, col);
                        }
                    });

            m_grid_layout->addWidget(button, row, col);
        }
    }

    m_game = new Game(this);

    // Difficulty buttons
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
            });

    m_heart_label->setGeometry(500, 50, 70, 50);
    m_heart_label->setAlignment(Qt::AlignCenter);
    m_heart_label->setStyleSheet("background-color: dimGray;");
    m_heart_label->setText("Hearts:");

    m_time_label->setText("00:00:00");
    m_time_label->setGeometry(600, 50, 70, 50);
    m_time_label->setAlignment(Qt::AlignCenter);
    m_time_label->setStyleSheet("background-color: dimGray;");

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
    m_heart_label->setText("Hearts: 3");
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
        QMessageBox::information(nullptr, "Win", "You won!!!");
    }
}

void MainWindow::changeHeartLabel()
{
    QMessageBox::information(nullptr, "wrong", "No!!!");

    m_heart_label->setText("Hearts: " + QString::number(m_game->getHearts()));
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