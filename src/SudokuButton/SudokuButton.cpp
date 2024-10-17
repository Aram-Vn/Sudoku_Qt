#include "../../include/SudokuButton/SudokuButton.h"

SudokuButton::SudokuButton(QWidget* parent)
    : QPushButton{ parent },
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
    
    if (!m_inputNumber.isEmpty())
    {
        QPainter painter(this);
        QFont font = painter.font();
        font.setPointSize(DEFAULT_CORNER_FONT_SIZE);
        painter.setFont(font);

        QRect textRect = rect();
        textRect.setTop(textRect.top() + DEFAULT_CORNER_PADDING);
        textRect.setRight(textRect.right() - DEFAULT_CORNER_PADDING);

        QColor textColor = painter.pen().color(); // Get the current text color

        if (textColor.value() > 128)
        {
            painter.setPen(textColor.darker(150));
        }
        else
        {
            painter.setPen(textColor.lighter(150));
        }

        painter.setOpacity(DEFAULT_CORNER_TEXT_OPACITY);
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignTop, m_inputNumber);
        painter.setOpacity(CORNER_FULL_OPACITY);
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
