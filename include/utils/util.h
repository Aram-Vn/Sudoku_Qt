#ifndef SUDOKU_QT_INCLUDE_UTILS_UTIL_H
#define SUDOKU_QT_INCLUDE_UTILS_UTIL_H

#include <QColor>
#include <QRegularExpression>
#include <QString>

namespace util {
    void    parseColors(const QString& stylesheet, QColor& baseColor, QColor& hoverColor, QColor& focusColor,
                        QString& textColor);
    QString colorStyleSet(const QColor& baseColor, const QColor& hoverColor, const QColor& focusColor,
                          const QString& textColor = "black");
} // namespace util

#endif // SUDOKU_QT_INCLUDE_UTILS_UTIL_H