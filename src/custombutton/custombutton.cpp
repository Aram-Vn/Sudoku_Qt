#include "../../include/custombutton/custombutton.h"
#include <QKeyEvent>
#include <QPainter>

CustomButton::CustomButton(QWidget* parent)
    : QPushButton(parent)
{
}

void CustomButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_clearOnNextKeyPress = true;
        emit leftClicked();
    }
    else if (event->button() == Qt::RightButton)
    {
        m_isListeningForKey = true;
        emit rightClicked();
    }

    QPushButton::mousePressEvent(event);
}

void CustomButton::keyPressEvent(QKeyEvent* event)
{
    if (m_isListeningForKey)
    {
        int key = event->key();

        if (key >= Qt::Key_1 && key <= Qt::Key_9)
        {
            m_inputNumber         = QString::number(key - Qt::Key_0); 
            m_isListeningForKey   = false;                            
            m_clearOnNextKeyPress = false;                           
            update();                                                 
        }
    }

    if (m_clearOnNextKeyPress)
    {
        m_inputNumber.clear();
        m_clearOnNextKeyPress = false;
        update();
    }

    QPushButton::keyPressEvent(event);
}

void CustomButton::paintEvent(QPaintEvent* event)
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

        painter.drawText(textRect, Qt::AlignRight | Qt::AlignTop, m_inputNumber);
    }
}

void CustomButton::clearNumber()
{
    m_inputNumber.clear();
    update(); 
}
