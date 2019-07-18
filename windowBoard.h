#ifndef WINDOWBOARD
#define WINDOWBOARD

#include <QFrame>
#include <QPointer>
#include <QtWidgets>
#include <string>
#include "board.h"
#include "game.h"

class WindowBoard: public QFrame {
    Q_OBJECT;

    public:
        // Initial set up of the board
        WindowBoard(QWidget* = 0, Game* = 0);

        // Make the board be aware of the next piece label,
        // so it can change it when need to
        void setNextPieceLabel(QPointer<QLabel> label);

        QSize sizeHint() const override;
        QSize minimumSizeHint() const override;

    signals:
        // signal sent to window after the command has been assessed
        void keyDown(std::string key);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

    private:
        enum { BoardWidth = 11, BoardHeight = 18 };
        QPointer<QLabel> nextPieceLabel;
        Game* game;
        void drawSquare(QPainter &painter, int x, int y, char blockType);
        int squareWidth() { return contentsRect().width() / BoardWidth; }
        int squareHeight() { return contentsRect().height() / BoardHeight; }
};

#endif