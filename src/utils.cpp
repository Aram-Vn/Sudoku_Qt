#include "../include/utils.h"

namespace util {
    void parseColors(const QString& stylesheet, QColor& baseColor, QColor& hoverColor, QColor& focusColor,
                     QString& textColor)
    {
        QRegularExpression baseColorRegex("background-color: (#[0-9a-fA-F]{6})");
        QRegularExpression textColorRegex("QPushButton \\{[^}]*color: ([a-zA-Z]+|#[0-9a-fA-F]{6})");
        QRegularExpression focusColorRegex("QPushButton:focus \\{[^}]*background-color: (#[0-9a-fA-F]{6})");
        QRegularExpression hoverColorRegex("QPushButton:hover \\{[^}]*background-color: (#[0-9a-fA-F]{6})");

        QRegularExpressionMatch baseMatch  = baseColorRegex.match(stylesheet);
        QRegularExpressionMatch textMatch  = textColorRegex.match(stylesheet);
        QRegularExpressionMatch focusMatch = focusColorRegex.match(stylesheet);
        QRegularExpressionMatch hoverMatch = hoverColorRegex.match(stylesheet);

        if (baseMatch.hasMatch())
        {
            baseColor = QColor(baseMatch.captured(1));
        }
        if (textMatch.hasMatch())
        {
            textColor = textMatch.captured(1);
        }
        if (focusMatch.hasMatch())
        {
            focusColor = QColor(focusMatch.captured(1));
        }
        if (hoverMatch.hasMatch())
        {
            hoverColor = QColor(hoverMatch.captured(1));
        }
    }

    QString colorStyleSet(const QColor& baseColor, const QColor& hoverColor, const QColor& focusColor,
                          const QString& textColor)
    {
        return QString("QPushButton {"
                       "background-color: %1;"
                       "color: %4;"
                       "border-radius: 8px;"
                       "font-size: 16px;"
                       "font-weight: bold;"
                       "}"
                       "QPushButton:focus {"
                       "background-color: %2;"
                       "border: 6px solid %3;"
                       "}"
                       "QPushButton:hover {"
                       "background-color: %2;"
                       "border: 2px solid black;"
                       "color: white;"
                       "border-radius: 10px;"
                       "}")
            .arg(baseColor.name())
            .arg(hoverColor.name())
            .arg(focusColor.name())
            .arg(textColor);
    }
} // namespace util
