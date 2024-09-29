#ifndef SUDOKU_QT_INCLUDE_UTILS_COLOR_UTIL_H
#define SUDOKU_QT_INCLUDE_UTILS_COLOR_UTIL_H

#include <QColor>
#include <QGridLayout>
#include <QPushButton>
#include <QRegularExpression>
#include <QString>
#include <qtypes.h>

namespace colorUtil {

    enum class buttonType : qint8
    {
        EASY = 0,
        MEDIUM,
        HARD,
        CUSTOM,
        LABEL,
        MAIN
    };

    QString getStyle(buttonType type);

    QString colorStyleSet(const QColor& baseColor, const QColor& hoverColor, const QColor& focusColor,
                          const QString& textColor = "black", const QString& FocusTextColor = "white");

    void applyColorStyles(QGridLayout* gridLayout, const QString& darkStyle, const QString& lightStyle);

} // namespace colorUtil

#endif // SUDOKU_QT_INCLUDE_UTILS_COLOR_UTIL_H