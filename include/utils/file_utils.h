#ifndef SUDOKU_QT_INCLUDE_UTILS_FILE_UTILS_H
#define SUDOKU_QT_INCLUDE_UTILS_FILE_UTILS_H

#include "../include/GameStateManager.h"
#include "color_utils.h"

#include <QColor>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTextStream>
#include <QVector>

namespace fileUtil {

    void writeInBinary(const QString& filePath, const QVector<QVector<int>>& board,
                       const QVector<QVector<int>>& fullBoard, int difficulty, int emptyCount, int heartCount,
                       quint32 seconds, const QString& darkStyle, const QString& lightStyle,
                       const QVector<QVector<QString>>& TopRightButtonNumbers);

    bool readFromBinary(const QString& filePath, QVector<QVector<int>>& board, QVector<QVector<int>>& fullBoard,
                        int& difficulty, int& emptyCount, int& heartCount, quint32& seconds, QString& darkStyle,
                        QString& lightStyle, QVector<QVector<QString>>& TopRightButtonNumbers);

    bool checkFile(const int i);

    void removeFile(const int i);

    void writeInJSON(const QString& filePath, const QVector<QVector<int>>& board,
                     const QVector<QVector<int>>& fullBoard, const int difficulty, const int emptyCount,
                     const int heartCount, const int seconds, const QString& darkStyle, const QString& lightStyle,
                     const QVector<QVector<QString>>& buttonNumbers);

    bool readFromJSON(const QString& filePath, QVector<QVector<int>>& board, QVector<QVector<int>>& fullBoard,
                      int& difficulty, int& emptyCount, int& heartCount, int& seconds, QString& darkStyle,
                      QString& lightStyle, QVector<QVector<QString>>& buttonNumbers);

} // namespace fileUtil

#endif // SUDOKU_QT_INCLUDE_UTILS_FILE_UTILS_H
