#include "../../include/utils/file_utils.h"

namespace fileUtil {

    void writeInJSON(const QString& filePath, const QVector<QVector<int>>& board,
                     const QVector<QVector<int>>& fullBoard, const int difficulty, const int emptyCount,
                     const int heartCount, const int seconds, const QString& darkStyle, const QString& lightStyle)
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QJsonObject json;

            // Save the current board
            QJsonArray boardArray;

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
            QJsonArray fullBoardArray;
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
            json["difficulty"] = difficulty;

            // Save the number of empty fields
            json["emptyFields"] = emptyCount;

            // Save the number of hearts
            json["hearts"] = heartCount;

            // Save the elapsed time
            json["time"] = seconds;

            // Save color state
            QJsonObject colorStyles;
            colorStyles["darkStyle"]  = darkStyle;
            colorStyles["lightStyle"] = lightStyle;

            json["colors"] = colorStyles;

            QJsonDocument doc(json);
            file.write(doc.toJson());
            file.close();
        }
    }

    bool readFromJSON(const QString& filePath, QVector<QVector<int>>& board, QVector<QVector<int>>& fullBoard,
                      int& difficulty, int& emptyCount, int& heartCount, int& seconds, QString& darkStyle,
                      QString& lightStyle)
    {
        QFile file(filePath);
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
        if (json.contains("board"))
        {
            QJsonArray boardArray = json["board"].toArray();
            board.resize(9);
            for (int row = 0; row < 9; ++row)
            {
                QJsonArray rowArray = boardArray[row].toArray();
                board[row].resize(9);
                for (int col = 0; col < 9; ++col)
                {
                    board[row][col] = rowArray[col].toInt();
                }
            }
        }
        else
        {
            qWarning() << "Missing 'board' in JSON.";
            return false;
        }

        // Load the full board (the solution)
        if (json.contains("fullBoard"))
        {
            QJsonArray fullBoardArray = json["fullBoard"].toArray();
            fullBoard.resize(9);
            for (int row = 0; row < 9; ++row)
            {
                QJsonArray rowArray = fullBoardArray[row].toArray();
                fullBoard[row].resize(9);
                for (int col = 0; col < 9; ++col)
                {
                    fullBoard[row][col] = rowArray[col].toInt();
                }
            }
        }

        // Load other data
        if (json.contains("difficulty"))
        {
            difficulty = json["difficulty"].toInt();
        }
        else
        {
            qWarning() << "Missing 'difficulty' in JSON.";
            return false;
        }

        if (json.contains("emptyFields"))
        {
            emptyCount = json["emptyFields"].toInt();
        }
        else
        {
            qWarning() << "Missing 'emptyFields' in JSON.";
            return false;
        }

        if (json.contains("hearts"))
        {
            heartCount = json["hearts"].toInt();
        }
        else
        {
            qWarning() << "Missing 'hearts' in JSON.";
            return false;
        }

        if (json.contains("time"))
        {
            seconds = json["time"].toInt();
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
            darkStyle               = colorStyles["darkStyle"].toString();
            lightStyle              = colorStyles["lightStyle"].toString();
        }
        else
        {
            qWarning() << "Missing 'colors' in JSON.";
            return false;
        }

        return true;
    }

    void applyColorStyles(QGridLayout* gridLayout, const QString& darkStyle, const QString& lightStyle)
    {
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                QPushButton* sudokuButton = dynamic_cast<QPushButton*>(gridLayout->itemAtPosition(row, col)->widget());

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

} // namespace fileUtil