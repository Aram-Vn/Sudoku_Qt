#ifndef SudokuButton_H
#define SudokuButton_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

class SudokuButton : public QPushButton
{
    Q_OBJECT

public:
    explicit SudokuButton(QWidget* parent = nullptr);

signals:
    void rightClicked();
    void leftClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public:
    void    clearNumber();
    QString getTopRightNumber() const;
    void    setTopRightNumber(const QString& number);

private:
    QString m_inputNumber;
    bool    m_isListeningForKey;
};

#endif // SudokuButton_H
