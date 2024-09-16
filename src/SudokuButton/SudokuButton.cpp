#include "../../include/SudokuButton/SudokuButton.h"

SudokuButton::SudokuButton(QWidget* parent)
    : QPushButton(parent),
      m_isListeningForKey{ false }
{
}

void SudokuButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit leftClicked();
    }
    else if (event->button() == Qt::RightButton)
    {
        m_isListeningForKey = true;
        emit rightClicked();
    }

    QPushButton::mousePressEvent(event);
}

void SudokuButton::keyPressEvent(QKeyEvent* event)
{
    if (m_isListeningForKey)
    {
        int key = event->key();

        if (key >= Qt::Key_1 && key <= Qt::Key_9)
        {
            m_inputNumber       = QString::number(key - Qt::Key_0);
            m_isListeningForKey = false;

            if (this->text().isEmpty())
            {
                update();
            }
            else
            {
                m_inputNumber.clear();
            }
        }
    }

    QPushButton::keyPressEvent(event);
}

void SudokuButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    if (!m_inputNumber.isEmpty())
    {
        QFont font = painter.font();
        font.setPointSize(12);
        painter.setFont(font);

        QRect textRect = rect();
        int   padding  = 5;
        textRect.setTop(textRect.top() + padding);
        textRect.setRight(textRect.right() - padding);

        QColor textColor = painter.pen().color(); // Get the current text color

        if (textColor.value() > 128)
        {
            painter.setPen(textColor.darker(150));
        }
        else
        {
            painter.setPen(textColor.lighter(150));
        }

        painter.setOpacity(0.5);
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignTop, m_inputNumber);
        painter.setOpacity(2.0);
    }
}

void SudokuButton::clearNumber()
{
    m_inputNumber.clear();
    update();
}

QString SudokuButton::getTopRightNumber() const
{
    return m_inputNumber;
}

void SudokuButton::setTopRightNumber(const QString& number)
{
    m_inputNumber = number;
    update();
}
