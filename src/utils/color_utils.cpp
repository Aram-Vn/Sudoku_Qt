#include "../../include/utils/color_utils.h"

namespace colorUtil {

    QString getStyle(buttonType type)
    {
        switch (type)
        {
            case buttonType::EASY:
                return "QPushButton {"
                       "background-color: #005700;"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "color: white;"
                       "font-family: 'Arial';"
                       "font-size: 16px;"
                       "font-weight: bold;"
                       "border: 2px solid #555;"
                       "text-align: center;"
                       "}"
                       "QPushButton:hover {"
                       "background-color: #007500;"
                       "}";
            case buttonType::MEDIUM:
                return "QPushButton {"
                       "background-color: #7f7f00;"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "color: black;"
                       "font-family: 'Arial';"
                       "font-size: 16px;"
                       "font-weight: bold;"
                       "border: 2px solid #555;"
                       "text-align: center;"
                       "}"
                       "QPushButton:hover {"
                       "background-color: #9f9f00;"
                       "}";
            case buttonType::HARD:
                return "QPushButton {"
                       "background-color: #7f0000;"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "color: white;"
                       "font-family: 'Arial';"
                       "font-size: 16px;"
                       "font-weight: bold;"
                       "border: 2px solid #555;"
                       "text-align: center;"
                       "}"
                       "QPushButton:hover {"
                       "background-color: #a70000;"
                       "}";
            case buttonType::CUSTOM:
                return "QPushButton {"
                       "background-color: #4f4f4f;"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "color: white;"
                       "font-family: 'Arial';"
                       "font-size: 16px;"
                       "font-weight: bold;"
                       "border: 2px solid #888;"
                       "text-align: center;"
                       "}"
                       "QPushButton:hover {"
                       "background-color: #7f7f7f;"
                       "}";
            case buttonType::LABEL:
                return "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                       "stop:0 #3e3e3e, stop:1 #2c2c2c);"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "color: white;"
                       "font-family: 'Arial';"
                       "font-size: 18px;"
                       "font-weight: bold;"
                       "border: 2px solid #555;"
                       "text-align: center;";
            case buttonType::MAIN:
                return "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                       "stop:0 #484848, "
                       "stop:0.33 #383838, "
                       "stop:0.66 #333333, "
                       "stop:1 #282828);";
            default: return "";
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
