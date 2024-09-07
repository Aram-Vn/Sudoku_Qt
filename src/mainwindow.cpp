#include "../include/mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_difficulty_buttons(3),
      m_start_button(new QPushButton(this)),
      m_continue_old_game(new QPushButton(this)),
      m_reset_game(new QPushButton(this)),
      m_heart_label(new QLabel(this)),
      m_time_label(new QLabel(this)),
      m_timer(new QTimer(this)),
      m_game(new Game(this))

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
            QPushButton* sudokuButton = new QPushButton(this);
            sudokuButton->setFixedSize(60, 60);

            bool is_dark = ((row / 3) % 2 == (col / 3) % 2);

            QColor baseColor  = is_dark ? QColor("#018AAF") : QColor("#4FD9FF");
            QColor hoverColor = baseColor.darker(150);
            QColor focusColor = hoverColor.darker(150);

            QString colorStyle = colorUtil::colorStyleSet(baseColor, hoverColor, focusColor);

            sudokuButton->setStyleSheet(colorStyle);
            sudokuButton->setEnabled(false);

            connect(sudokuButton, &QPushButton::clicked, this,
                    [row, col, sudokuButton, this]()
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

    m_continue_old_game->setText("Continue old game");
    m_continue_old_game->setStyleSheet("background-color: dimGray;"
                                       "text-align: left;"
                                       "padding-left: 8px;"); // Adjust padding as needed
    m_continue_old_game->setGeometry(0, 0, 145, 40);

    connect(m_continue_old_game, &QPushButton::clicked, this, [this]() { promptContinueOldGame(); });
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
    int     hearts    = m_game->getHearts();
    QString heartText = QString("Hearts: ");

    for (int i = 0; i < hearts; ++i)
    {
        heartText += "<span style='color:red;'>♥&nbsp;</span>";
    }

    m_heart_label->setText(heartText.trimmed());

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
            QColor baseColor  = is_dark ? colorDark : colorLight;
            QColor hoverColor = baseColor.darker(150);
            QColor focusColor = hoverColor.darker(150);

            // Determine if the color is too dark for black text
            QString textColor = (baseColor.lightness() < 128) ? "white" : "black";

            QString colorStyle = colorUtil::colorStyleSet(baseColor, hoverColor, focusColor, textColor);

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
    QFile file("sudoku_save.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject json;

        // Save the current board
        QVector<QVector<int>> board = m_game->getBoard();
        QJsonArray            boardArray;
        
        for (int row = 0; row < 9; ++row)
        {
            QJsonArray rowArray;
            for (int col = 0; col < 9; ++col)
            {
                rowArray.append(board[row][col]);
            }
            boardArray.append(rowArray);
        }
        json["board"] = boardArray;

        // Save the full board (the solution)
        QVector<QVector<int>> fullBoard = m_game->getFullBoard();
        QJsonArray            fullBoardArray;
        if (!fullBoard.empty())
        {
            for (int row = 0; row < 9; ++row)
            {
                QJsonArray rowArray;
                for (int col = 0; col < 9; ++col)
                {
                    rowArray.append(fullBoard[row][col]);
                }
                fullBoardArray.append(rowArray);
            }
            json["fullBoard"] = fullBoardArray;
        }

        // Save the difficulty level
        json["difficulty"] = m_game->getDifficulty();

        // Save the number of empty fields
        json["emptyFields"] = m_game->getEmptyCount();

        // Save the number of hearts
        json["hearts"] = m_game->getHearts();

        // Save the elapsed time
        json["time"] = m_seconds;

        // Save color state
        QJsonObject  colorStyles;
        QPushButton* sudokuButton1 = dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(0, 0)->widget());
        colorStyles["color1"]      = sudokuButton1->styleSheet();

        QPushButton* sudokuButton2 = dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(0, 4)->widget());
        colorStyles["color2"]      = sudokuButton2->styleSheet();

        json["colors"] = colorStyles;

        QJsonDocument doc(json);
        file.write(doc.toJson());
        file.close();
    }
}

bool MainWindow::loadGameState()
{
    QFile file("sudoku_save.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file for reading.";
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(data));
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Invalid JSON document.";
        return false;
    }

    QJsonObject json = doc.object();

    // Load the current board
    QVector<QVector<int>> board(9, QVector<int>(9, 0));
    if (json.contains("board"))
    {
        QJsonArray boardArray = json["board"].toArray();
        for (int row = 0; row < 9; ++row)
        {
            QJsonArray rowArray = boardArray[row].toArray();
            for (int col = 0; col < 9; ++col)
            {
                board[row][col] = rowArray[col].toInt();
            }
        }
        m_game->setBoard(board);
    }
    else
    {
        qWarning() << "Missing 'board' in JSON.";
        return false;
    }

    // Load the full board (the solution)
    QVector<QVector<int>> fullBoard(9, QVector<int>(9, 0));
    if (json.contains("fullBoard"))
    {
        QJsonArray fullBoardArray = json["fullBoard"].toArray();
        for (int row = 0; row < 9; ++row)
        {
            QJsonArray rowArray = fullBoardArray[row].toArray();
            for (int col = 0; col < 9; ++col)
            {
                fullBoard[row][col] = rowArray[col].toInt();
            }
        }
        m_game->setFullBoard(fullBoard);
    }

    // Load difficulty level
    if (json.contains("difficulty"))
    {
        int difficulty = json["difficulty"].toInt();
        m_game->setDifficulty(difficulty);
    }
    else
    {
        qWarning() << "Missing 'difficulty' in JSON.";
        return false;
    }

    // Load number of empty fields
    if (json.contains("emptyFields"))
    {
        int emptyFields = json["emptyFields"].toInt();
        m_game->setEmptyCount(emptyFields);
    }
    else
    {
        qWarning() << "Missing 'emptyFields' in JSON.";
        return false;
    }

    // Load number of hearts
    if (json.contains("hearts"))
    {
        int hearts = json["hearts"].toInt();
        m_game->setHearts(hearts);
    }
    else
    {
        qWarning() << "Missing 'hearts' in JSON.";
        return false;
    }

    // Load elapsed time
    if (json.contains("time"))
    {
        m_seconds = json["time"].toInt();
        m_time_label->setText(QString("%1:%2:%3")
                                  .arg(m_seconds / 3600, 2, 10, QLatin1Char('0'))
                                  .arg((m_seconds % 3600) / 60, 2, 10, QLatin1Char('0'))
                                  .arg(m_seconds % 60, 2, 10, QLatin1Char('0')));
        m_timer->start();
    }
    else
    {
        qWarning() << "Missing 'time' in JSON.";
        return false;
    }

    // Load colors
    if (json.contains("colors"))
    {
        QJsonObject colorStyles = json["colors"].toObject();
        QString     darkStyle   = colorStyles["color1"].toString();
        QString     lightStyle  = colorStyles["color2"].toString();

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                QPushButton* sudokuButton =
                    dynamic_cast<QPushButton*>(m_grid_layout->itemAtPosition(row, col)->widget());

                bool    is_dark    = ((row / 3) % 2 == (col / 3) % 2);
                QString colorStyle = is_dark ? darkStyle : lightStyle;

                QColor  baseColor;
                QColor  hoverColor;
                QColor  focusColor;
                QString textColor;

                colorUtil::parseColors(colorStyle, baseColor, hoverColor, focusColor, textColor);

                QString colorStyleString = colorUtil::colorStyleSet(baseColor, hoverColor, focusColor, textColor);
                sudokuButton->setStyleSheet(colorStyleString);
            }
        }
    }
    else
    {
        qWarning() << "Missing 'colors' in JSON.";
        return false;
    }

    return true;
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
        resetGame();
    }
}
