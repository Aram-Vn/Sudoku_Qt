#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QVector>

namespace fileUtil {

    QStringList           readFileLines(const QString& filePath);
    QVector<QVector<int>> parseBoard(const QStringList& lines, int startIndex, int rows = 9);
    QString               parseTextLine(const QStringList& lines, int& currentLine, const QString& prefix);
    
} // namespace fileUtil

#endif // FILE_UTILS_H
