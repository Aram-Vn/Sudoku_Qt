#include "../../include/utils/color_utils.h"

namespace colorUtil {
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
                          const QString& textColor, const QString& FocusTextColor)
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
                       "color: %5;"
                       "border-radius: 10px;"
                       "}")
            .arg(baseColor.name())
            .arg(hoverColor.name())
            .arg(focusColor.name())
            .arg(textColor)
            .arg(FocusTextColor);
    }

    void applyColorStyles(QGridLayout* gridLayout, const QString& darkStyle, const QString& lightStyle)
    {
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                QPushButton* sudokuButton = dynamic_cast<QPushButton*>(gridLayout->itemAtPosition(row, col)->widget());

                bool    is_dark    = ((row / 3) % 2 == (col / 3) % 2);
                QString colorStyle = is_dark ? darkStyle : lightStyle;

                QColor  baseColor;
                QColor  hoverColor;
                QColor  focusColor;
                QString textColor;

                sudokuButton->setStyleSheet(colorStyle);
            }
        }
    }
} // namespace colorUtil
