#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QVector>

namespace fileUtil {

    void writeInJSON(QVector<QVector<int>>& board, QVector<QVector<int>>& fullBoard, const int difficulty,
                     const int emptyCount, const int heartCount, const int seconds, QString& darkStyle,
                     QString& lightStyle);

} // namespace fileUtil

#endif // FILE_UTILS_H
