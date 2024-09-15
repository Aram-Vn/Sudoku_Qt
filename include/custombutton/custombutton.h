#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QKeyEvent>
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
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public:
    void clearNumber();

private:
    QString m_inputNumber;
    bool    m_isListeningForKey;
};

#endif // CUSTOMBUTTON_H
