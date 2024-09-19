#include "../../include/utils/file_utils.h"

namespace fileUtil {

    void writeInBinary(const QString& filePath, const QVector<QVector<int>>& board,
                       const QVector<QVector<int>>& fullBoard, int difficulty, int emptyCount, int heartCount,
                       int seconds, const QString& darkStyle, const QString& lightStyle,
                       const QVector<QVector<QString>>& TopRightButtonNumbers)
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);

            // Write the current board
            for (int row = 0; row < 9; ++row)
            {
                for (int col = 0; col < 9; ++col)
                {
                    out << (quint8)board[row][col]; 
                }
            }

            // Write the full board
            for (int row = 0; row < 9; ++row)
            {
                for (int col = 0; col < 9; ++col)
                {
                    out << (quint8)fullBoard[row][col]; 
                }
            }

            // Write difficulty, emptyCount, heartCount, and time
            out << (quint8)difficulty;
            out << (quint8)emptyCount;
            out << (quint8)heartCount;
            out << (quint32)seconds;

            // Write darkStyle and lightStyle
            out << darkStyle << lightStyle;

            // Write button numbers 
            for (int row = 0; row < 9; ++row)
            {
                for (int col = 0; col < 9; ++col)
                {
                    out << TopRightButtonNumbers[row][col];
                }
            }

            file.close();
        }
    }

    bool readFromBinary(const QString& filePath, QVector<QVector<int>>& board, QVector<QVector<int>>& fullBoard,
                        int& difficulty, int& emptyCount, int& heartCount, int& seconds, QString& darkStyle,
                        QString& lightStyle, QVector<QVector<QString>>& TopRightButtonNumbers)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Unable to open file for reading.";
            return false;
        }

        QDataStream in(&file);

        // Read the current board
        board.resize(9);
        for (int row = 0; row < 9; ++row)
        {
            board[row].resize(9);
            for (int col = 0; col < 9; ++col)
            {
                quint8 value;
                in >> value;
                board[row][col] = value;
            }
        }

        // Read the full board (solution)
        fullBoard.resize(9);
        for (int row = 0; row < 9; ++row)
        {
            fullBoard[row].resize(9);
            for (int col = 0; col < 9; ++col)
            {
                quint8 value;
                in >> value;
                fullBoard[row][col] = value;
            }
        }

        // Read difficulty, emptyCount, heartCount, and time
        quint8  difficultyValue, emptyCountValue, heartCountValue;
        quint32 secondsValue;

        in >> difficultyValue;
        in >> emptyCountValue;
        in >> heartCountValue;
        in >> secondsValue;

        difficulty = difficultyValue;
        emptyCount = emptyCountValue;
        heartCount = heartCountValue;
        seconds    = secondsValue;

        // Read darkStyle and lightStyle
        in >> darkStyle >> lightStyle;

        // Read button numbers
        TopRightButtonNumbers.resize(9);
        for (int row = 0; row < 9; ++row)
        {
            TopRightButtonNumbers[row].resize(9);
            for (int col = 0; col < 9; ++col)
            {
                QString buttonText;
                in >> buttonText; // Read the QString
                TopRightButtonNumbers[row][col] = buttonText;
            }
        }

        file.close();
        return true;
    }

    void writeInJSON(const QString& filePath, const QVector<QVector<int>>& board,
                     const QVector<QVector<int>>& fullBoard, const int difficulty, const int emptyCount,
                     const int heartCount, const int seconds, const QString& darkStyle, const QString& lightStyle,
                     const QVector<QVector<QString>>& TopRightButtonNumbers)
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

            // Save the board top right numbers
            QJsonArray TopRightButtonNumbersArray;

            for (int row = 0; row < 9; ++row)
            {
                QJsonArray rowArray;
                for (int col = 0; col < 9; ++col)
                {
                    rowArray.append(TopRightButtonNumbers[row][col]);
                }
                TopRightButtonNumbersArray.append(rowArray);
            }

            json["TopRightButtonNumbers"] = TopRightButtonNumbersArray;

            QJsonDocument doc(json);
            file.write(doc.toJson());
            file.close();
        }
    }

    bool readFromJSON(const QString& filePath, QVector<QVector<int>>& board, QVector<QVector<int>>& fullBoard,
                      int& difficulty, int& emptyCount, int& heartCount, int& seconds, QString& darkStyle,
                      QString& lightStyle, QVector<QVector<QString>>& TopRightButtonNumbers)
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
        else
        {
            return false;
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

        if (json.contains("TopRightButtonNumbers"))
        {
            QJsonArray TopRightButtonNumbersArray = json["TopRightButtonNumbers"].toArray();
            TopRightButtonNumbers.resize(9);
            for (int row = 0; row < 9; ++row)
            {
                QJsonArray rowArray = TopRightButtonNumbersArray[row].toArray();
                TopRightButtonNumbers[row].resize(9);
                for (int col = 0; col < 9; ++col)
                {
                    TopRightButtonNumbers[row][col] = rowArray[col].toString();
                }
            }
        }
        else
        {
            {
                return false;
            }
        }

        return true;
    }

} // namespace fileUtil