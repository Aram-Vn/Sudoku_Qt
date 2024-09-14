#include "../../include/custombutton/custombutton.h"

CustomButton::CustomButton(QWidget* parent)
    : QPushButton(parent)
{
}

void CustomButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit leftClicked(); // Emit signal for left-click
    }
    else if (event->button() == Qt::RightButton)
    {
        emit rightClicked(); // Emit signal for right-click
    }

    // Optionally call the base class method if needed
    QPushButton::mousePressEvent(event);
}