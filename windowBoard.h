#ifndef WINDOWBOARD
#define WINDOWBOARD

#include <QFrame>
#include "board.h"
#include "game.h"

class WindowBoard: public QFrame {
    Q_OBJECT;

    public:
        WindowBoard(QWidget* = 0);
        WindowBoard(QWidget* = 0, Game* = 0);

        QSize sizeHint() const override;
        QSize minimumSizeHint() const override;
    protected:
        void paintEvent(QPaintEvent *event) override;
        // void keyPressEvent(QKeyEvent *event) override;

    private:
        enum { BoardWidth = 11, BoardHeight = 18 };
        Game* game;
        void drawSquare(QPainter &painter, int x, int y, char blockType);
        int squareWidth() { return contentsRect().width() / BoardWidth; }
        int squareHeight() { return contentsRect().height() / BoardHeight; }
};

#endif