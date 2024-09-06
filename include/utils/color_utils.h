#ifndef SUDOKU_QT_INCLUDE_UTILS_COLORUTIL_H
#define SUDOKU_QT_INCLUDE_UTILS_COLORUTIL_H

#include <QColor>
#include <QRegularExpression>
#include <QString>

namespace colorUtil {
    void    parseColors(const QString& stylesheet, QColor& baseColor, QColor& hoverColor, QColor& focusColor,
                        QString& textColor);
    QString colorStyleSet(const QColor& baseColor, const QColor& hoverColor, const QColor& focusColor,
                          const QString& textColor = "black");
} // namespace colorUtil

#endif // SUDOKU_QT_INCLUDE_UTILS_COLORUTIL_H