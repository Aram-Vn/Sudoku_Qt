#ifndef SUDOKU_QT_INCLUDE_UTILS_COLORUTIL_H
#define SUDOKU_QT_INCLUDE_UTILS_COLORUTIL_H

#include <QColor>
#include <QGridLayout>
#include <QPushButton>
#include <QRegularExpression>
#include <QString>
#include <qcontainerfwd.h>

namespace colorUtil {
    void parseColors(const QString& stylesheet, QColor& baseColor, QColor& hoverColor, QColor& focusColor,
                     QString& textColor);

    QString colorStyleSet(const QColor& baseColor, const QColor& hoverColor, const QColor& focusColor,
                          const QString& textColor = "black", const QString& FocusTextColor = "white");

    void applyColorStyles(QGridLayout* gridLayout, const QString& darkStyle, const QString& lightStyle);

} // namespace colorUtil

#endif // SUDOKU_QT_INCLUDE_UTILS_COLORUTIL_H