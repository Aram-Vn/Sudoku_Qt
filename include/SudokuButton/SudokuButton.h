#ifndef SUDOKU_QT_INCLUDE_SUDOKU_BUTTON_SUDOKU_BUTTON_H
#define SUDOKU_QT_INCLUDE_SUDOKU_BUTTON_SUDOKU_BUTTON_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <cstddef>

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
    static constexpr std::size_t DEFAULT_CORNER_FONT_SIZE    = 15;
    static constexpr int         DEFAULT_CORNER_PADDING      = 5;
    static constexpr double      DEFAULT_CORNER_TEXT_OPACITY = 0.5;
    static constexpr double      CORNER_FULL_OPACITY         = 1.0;

private:
    QString m_inputNumber;
    bool    m_isListeningForKey;
};

#endif // SUDOKU_QT_INCLUDE_SUDOKU_BUTTON_SUDOKU_BUTTON_H
