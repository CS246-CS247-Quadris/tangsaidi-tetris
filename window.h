#ifndef WINDOW
#define WINDOW

#include <QFrame>
#include <QWidget>
#include <QPointer>
#include <memory>

#include "game.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Window: public QWidget {
    Q_OBJECT
    // window does not own the game
    
    public:
        Window(Game*);

    // public slots:
	// 	void repaint();

    private:
        std::unique_ptr<QLabel> createLabel(const QString &text);

        Game* game;

        QPointer<QFrame> dropZone;

        QPointer<QLabel> nextPieceLabel;
        QPointer<QLCDNumber> scoreLcd;
        QPointer<QLCDNumber> levelLcd;
        QPointer<QLCDNumber> hiLcd;
        QPointer<QPushButton> restartButton;
        QPointer<QPushButton> quitButton;
};

#endif