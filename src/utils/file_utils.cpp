#include "../../include/utils/file_utils.h"
#include <qobject.h>

namespace fileUtil {

    void writeInBinary(const QString& filePath, const QVector<QVector<int>>& board,
                       const QVector<QVector<int>>& fullBoard, int difficulty, int emptyCount, int heartCount,
                       quint32 seconds, const QString& darkStyle, const QString& lightStyle,
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

            if (fullBoard.size() == 9)
            {
                // Write the full board
                for (int row = 0; row < 9; ++row)
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        out << (quint8)fullBoard[row][col];
                    }
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
                        int& difficulty, int& emptyCount, int& heartCount, quint32& seconds, QString& darkStyle,
                        QString& lightStyle, QVector<QVector<QString>>& TopRightButtonNumbers)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Unable to open file for reading.";
            return false;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);

        if (file.size() < 9 * 9 * 2 + 4 + 2) // Minimum size for boards, settings, and styles
        {
            qWarning() << "File is too small to contain all required data.";
            file.close();
            return false;
        }

        // current board
        board.resize(9);
        for (int row = 0; row < 9; ++row)
        {
            board[row].resize(9);
            for (int col = 0; col < 9; ++col)
            {
                quint8 value;
                if (in.atEnd())
                {
                    qWarning() << "Unexpected end of file while reading current board.";
                    file.close();
                    return false;
                }
                in >> value;
                if (value > 9) // Sudoku values should be 0-9
                {
                    qWarning() << "Invalid value in current board:" << value;
                    file.close();
                    return false;
                }
                board[row][col] = value;
            }
        }

        // full board (solution)
        fullBoard.resize(9);
        for (int row = 0; row < 9; ++row)
        {
            fullBoard[row].resize(9);
            for (int col = 0; col < 9; ++col)
            {
                quint8 value;
                if (in.atEnd())
                {
                    qWarning() << "Unexpected end of file while reading full board.";
                    file.close();
                    return false;
                }
                in >> value;
                if (value < 1 || value > 9)
                {
                    qWarning() << "Invalid value in full board:" << value;
                    file.close();
                    return false;
                }
                fullBoard[row][col] = value;
            }
        }

        quint8  difficultyValue, emptyCountValue, heartCountValue;
        quint32 secondsValue;

        if (in.atEnd())
        {
            qWarning() << "Unexpected end of file while reading game settings.";
            file.close();
            return false;
        }
        in >> difficultyValue >> emptyCountValue >> heartCountValue >> secondsValue;

        if (difficultyValue > 2 || emptyCountValue < 0 || emptyCountValue > 81 || heartCountValue < 0 ||
            heartCountValue > 3)
        {
            qWarning() << "Invalid game settings values.";
            file.close();
            return false;
        }

        difficulty = difficultyValue;
        emptyCount = emptyCountValue;
        heartCount = heartCountValue;
        seconds    = secondsValue;

        if (in.atEnd())
        {
            qWarning() << "Unexpected end of file while reading styles.";
            file.close();
            return false;
        }
        in >> darkStyle >> lightStyle;

        if (darkStyle.isEmpty() || lightStyle.isEmpty())
        {
            qWarning() << "Invalid style strings.";
            file.close();
            return false;
        }

        TopRightButtonNumbers.resize(9);
        for (int row = 0; row < 9; ++row)
        {
            TopRightButtonNumbers[row].resize(9);
            for (int col = 0; col < 9; ++col)
            {
                QString buttonText;
                if (in.atEnd())
                {
                    qWarning() << "Unexpected end of file while reading button numbers.";
                    file.close();
                    return false;
                }
                in >> buttonText;
                // Validate button text (adjust this check as needed)
                if (buttonText.length() > 3)
                {
                    qWarning() << "Invalid button text:" << buttonText;
                    file.close();
                    return false;
                }
                TopRightButtonNumbers[row][col] = buttonText;
            }
        }

        // Check if read all the data and reached the end of the file
        if (!in.atEnd())
        {
            qWarning() << "File contains more data than expected.";
            file.close();
            return false;
        }

        file.close();
        return true;
    }

    bool checkFile(const int i)
    {
        QString fileName;

        switch (i)
        {
            case 0: fileName = "/sudoku_save_0.bin"; break;
            case 1: fileName = "/sudoku_save_1.bin"; break;
            case 2: fileName = "/sudoku_save_2.bin"; break;
            default: fileName = "NONAME";
        }

        QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(filePath);
        filePath += fileName;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Unable to open file for reading.";
            return false;
        }


        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);

        if (file.size() < 9 * 9 * 2 + 4 + 2) // Minimum size for boards, settings, and styles
        {
            qWarning() << "File is too small to contain all required data.";
            file.close();
            return false;
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