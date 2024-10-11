#include "../../include/utils/color_utils.h"
#include <qcolor.h>
#include <qcontainerfwd.h>
#include <qdebug.h>

namespace colorUtil {

    QString getStyle(buttonType type)
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
                background_color       = "#4f4f4f";
                hover_background_color = "#7f7f7f";
                text_color             = "white";
                break;
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

        QString qc = QString("QPushButton {"
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

        return qc;
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

    QString colorStyleSet(const QColor& baseColor)
    {
        QColor  hoverColor;
        QColor  focusColor;
        QString textColor;
        QString focusTextColor;

        if (baseColor == Qt::black || baseColor.lightness() < 10)
        {
            // If the base color is black, set hover and focus colors to dark gray
            hoverColor     = QColor(50, 50, 50);    // Dark gray for hover
            focusColor     = QColor(100, 100, 100); // Lighter gray for focus
            textColor      = "white";
            focusTextColor = "black";
        }
        else if (baseColor.lightness() < 128)
        {
            // Make hover and focus colors lighter if base color is dark
            hoverColor     = baseColor.lighter(120);
            focusColor     = hoverColor.lighter(120);
            textColor      = "white";
            focusTextColor = "black";
        }
        else
        {
            // Make hover and focus colors darker if base color is light
            hoverColor     = baseColor.darker(150);
            focusColor     = hoverColor.darker(150);
            textColor      = "black";
            focusTextColor = "white";
        }

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
            .arg(focusTextColor)                     // 5
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
