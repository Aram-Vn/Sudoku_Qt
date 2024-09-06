#include "../../include/utils/file_utils.h"

namespace fileUtil {

    QStringList readFileLines(const QString& filePath)
    {
        QFile       file(filePath);
        QStringList lines;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {
                lines.append(in.readLine());
            }
            file.close();
        }
        else
        {
            qWarning() << "Failed to open file: " << filePath;
        }

        return lines;
    }

    QVector<QVector<int>> parseBoard(const QStringList& lines, int startIndex, int rows)
    {
        QVector<QVector<int>> board(rows, QVector<int>(rows, 0));
        for (int row = 0; row < rows && startIndex < lines.size(); ++row)
        {
            QStringList rowValues = lines[startIndex].split(' ');
            rowValues.removeAll(""); // Remove empty parts
            for (int col = 0; col < rows && col < rowValues.size(); ++col)
            {
                board[row][col] = rowValues[col].toInt();
            }
            ++startIndex;
        }
        return board;
    }

    QString parseTextLine(const QStringList& lines, int& currentLine, const QString& prefix)
    {
        QString result;
        if (currentLine < lines.size() && lines[currentLine].startsWith(prefix))
        {
            result = lines[currentLine].mid(prefix.length());
            ++currentLine;
        }
        return result;
    }

} // namespace fileUtil