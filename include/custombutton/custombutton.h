#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QMouseEvent>
#include <QPushButton>

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CustomButton(QWidget* parent = nullptr);

signals:
    void rightClicked();
    void leftClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // CUSTOMBUTTON_H
