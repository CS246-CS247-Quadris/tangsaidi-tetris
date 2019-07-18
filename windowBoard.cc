#include "windowBoard.h"
#include <map>

WindowBoard::WindowBoard(QWidget *parent, Game *game) : QFrame(parent), game(game) {
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
}


void WindowBoard::setNextPieceLabel(QPointer<QLabel> label) {
    nextPieceLabel = label;
}


QSize WindowBoard::sizeHint() const{
    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);
}

QSize WindowBoard::minimumSizeHint() const{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

void WindowBoard::paintEvent(QPaintEvent *event){
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    int boardTop = rect.bottom() - BoardHeight * squareHeight();

    int curRow = 0;
    int curCol = 0;
    // Paint the board based on what is printed on the text display
    for (char c: game->game->getStringified()) {
        if (c == '\n') {
            curCol = 0;
            curRow++;
        } else {
            if (c != ' ') {
                drawSquare(painter, rect.left() + curCol * squareWidth(),
                           boardTop + curRow * squareHeight(), c);
            }
            curCol++;
        }
    }

    if (nextPieceLabel) {
        // paints the next piece label according to the text display as well
        std::string nextBlock = game->game->getStringifiedNext();
        char blockType = nextBlock[nextBlock.size() - 1];

        int width, height;

        if (blockType == 'O') {
            width = 2;
            height = 2;
        } else if (blockType == 'I') {
            width = 4;
            height = 1;
        } else {
            width = 3;
            height = 2;
        }

        QPixmap pixmap(width * squareWidth(), height * squareHeight());
        QPainter painter(&pixmap);
        painter.fillRect(pixmap.rect(),nextPieceLabel->palette().background());

        int x = 0;
        int y = 0;
        for (char c: nextBlock) {
            if (c == '\n') {
                y++;
                x = 0;
            } else if (c == ' ') {
                x++;
            } else {
                drawSquare(painter, x * squareWidth(), y * squareHeight(), c);
                x++;
            }
        }

        nextPieceLabel->setPixmap(pixmap);
    }

}

void WindowBoard::drawSquare(QPainter &painter, int x, int y, char blockType){
    // Color scheme for each block type
    static const std::map<char, QRgb> colorTable = {{'I', 0xCC6666}, 
        {'J', 0x66CC66}, {'L', 0x6666CC}, {'O', 0xCCCC66}, 
        {'S', 0xCC66CC}, {'Z', 0x66CCCC}, {'T', 0xDAAA00},
        {'*', 0x663300}, {'?', 0x000000}};

    QColor color = colorTable.at(blockType);
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);

    painter.setPen(color.light());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.dark());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

void WindowBoard::keyPressEvent(QKeyEvent *event){
    // Translates keys to commands
    switch (event->key()) {
    case Qt::Key_Left:
        emit keyDown("left");
        break;
    case Qt::Key_Right:
        emit keyDown("right");
        break;
    case Qt::Key_Down:
        emit keyDown("clockwise");
        break;
    case Qt::Key_Up:
        emit keyDown("counterclockwise");
        break;
    case Qt::Key_Space:
        emit keyDown("drop");
        break;
    case Qt::Key_D:
        emit keyDown("down");
        break;
    case Qt::Key_I:
        emit keyDown("I");
        break;
    case Qt::Key_J:
        emit keyDown("J");
        break;
    case Qt::Key_L:
        emit keyDown("L");
        break;
    case Qt::Key_O:
        emit keyDown("O");
        break;
    case Qt::Key_S:
        emit keyDown("S");
        break;
    case Qt::Key_Z:
        emit keyDown("Z");
        break;
    case Qt::Key_T:
        emit keyDown("T");
        break;
    case Qt::Key_H:
        emit keyDown("hint");
        break;
    case Qt::Key_Minus:
        emit keyDown("leveldown");
        break;
    case Qt::Key_Plus:
        emit keyDown("levelup");
        break;
    default:
        QFrame::keyPressEvent(event);
    }
}