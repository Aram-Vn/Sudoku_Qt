#include "../../include/utils/color_utils.h"

namespace colorUtil {

    {
        QString background_color       = "";
        QString hover_background_color = "";
        QString text_color             = "";

        switch (type)
        {
            case buttonType::EASY:
                background_color       = "#005700";
                hover_background_color = "#007500";
                text_color             = "white";
                break;
            case buttonType::MEDIUM:
                background_color       = "#7f7f00";
                hover_background_color = "#9f9f00";
                text_color             = "black";
                break;
            case buttonType::HARD:
                background_color       = "#7f0000";
                hover_background_color = "#a70000";
                text_color             = "white";
                break;
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
                break; // Add break here
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
                break; // Add break here
            case buttonType::MAIN:
                return "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                       "stop:0 #484848, "
                       "stop:0.33 #383838, "
                       "stop:0.66 #333333, "
                       "stop:1 #282828);";
                break; // Add break here
            default: break;
        }

        return QString("QPushButton {"
                       "background-color: %2;"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "color: %4;"
                       "font-family: 'Arial';"
                       "font-size: %1px;"
                       "font-weight: bold;"
                       "border: 2px solid #555;"
                       "text-align: center;"
                       "}"
                       "QPushButton:hover {"
                       "background-color: %3;"
                       "}")
            .arg(DIFFICULTY_BUTTONS_FONT_SIZE) // 1
            .arg(background_color)             // 2
            .arg(hover_background_color)       // 3
            .arg(text_color);                  // 4
    }

    QString generateHeartSpan(int heartCount, QString imgPath, int width, int height)
    {
        QString heartSpan;
        for (int i = 0; i < heartCount; ++i)
        {
            heartSpan += QString("<img src='%1' width='%2' height='%3'>").arg(imgPath).arg(width).arg(height);
        }
        return heartSpan;
    }

    QString colorStyleSet(const QColor& baseColor, const QColor& hoverColor, const QColor& focusColor,
                          const QString& textColor, const QString& FocusTextColor)
    {
        return QString("QPushButton {"
                       "background-color: %1;"
                       "color: %4;"
                       "border-radius: 8px;"
                       "font-size: %6px;"
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
            .arg(baseColor.name())                   // 1
            .arg(hoverColor.name())                  // 2
            .arg(focusColor.name())                  // 3
            .arg(textColor)                          // 4
            .arg(FocusTextColor)                     // 5
            .arg(fontSize::SUDOKU_BUTTON_FONT_SIZE); // 6
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
