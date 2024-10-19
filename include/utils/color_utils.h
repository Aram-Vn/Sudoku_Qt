#ifndef SUDOKU_QT_INCLUDE_UTILS_COLOR_UTIL_H
#define SUDOKU_QT_INCLUDE_UTILS_COLOR_UTIL_H

#include <QColor>
#include <QGridLayout>
#include <QPushButton>
#include <QRegularExpression>
#include <QString>

namespace colorUtil {

    enum class widgetType : qint8
    {
        EASY_BUTTON = 0,
        MEDIUM_BUTTON,
        HARD_BUTTON,
        CUSTOM_BUTTON,
        LABEL,
        MAIN_LAYOUT
    };

    enum fontSize : qint8
    {
        DIFFICULTY_BUTTONS_FONT_SIZE = 16,
        SUDOKU_BUTTON_FONT_SIZE      = 24
    };

    constexpr int DEFAULT_HEART_COUNT        = 3;
    constexpr int DEFAULT_HEART_WIDTH_HEIGHT = 22;
    const QString DEFAULT_HEART_IMAGE_PATH   = ":/assets/Heart.png";

    QString getStyle(widgetType type);

    QString generateHeartSpan(int heartCount = DEFAULT_HEART_COUNT, QString imgPath = DEFAULT_HEART_IMAGE_PATH,
                              int width = DEFAULT_HEART_WIDTH_HEIGHT, int height = DEFAULT_HEART_WIDTH_HEIGHT);

    QString colorStyleSet(const QColor& baseColor);

    void applyColorStyles(QGridLayout* gridLayout, const QString& darkStyle, const QString& lightStyle);

} // namespace colorUtil

#endif // SUDOKU_QT_INCLUDE_UTILS_COLOR_UTIL_H