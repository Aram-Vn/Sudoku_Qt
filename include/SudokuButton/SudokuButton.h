#ifndef SUDOKU_QT_INCLUDE_SUDOKU_BUTTON_SUDOKU_BUTTON_H
#define SUDOKU_QT_INCLUDE_SUDOKU_BUTTON_SUDOKU_BUTTON_H

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

#endif // SUDOKU_QT_INCLUDE_SUDOKU_BUTTON_SUDOKU_BUTTON_H
