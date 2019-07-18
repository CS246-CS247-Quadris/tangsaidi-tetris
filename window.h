#ifndef WINDOW
#define WINDOW

#include <QFrame>
#include <QWidget>
#include <QPointer>
#include <memory>

#include "game.h"
#include "windowBoard.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Window: public QWidget {
    Q_OBJECT
    
    public:
        /*
        Constructs window object so it is also aware of game
         */
        Window(Game*);

    signals:
        /*
        Sends execCommand to text game
         */
        void execCommand(std::string command);
    
    public slots:
        /*
        Does a repaint in case something changed in the window
         */
		void updateWindow();
        /*
        Handles keyDown events, sends the command received from
        key pressed in board to text game
         */
        void keyDown(std::string command);

    private slots:
        /*
        Listener for restart button
         */
        void handleRestart();

    protected:
        /*
        Overrides the default paint methods, describes how the
        window should be painted
         */
        void paintEvent(QPaintEvent *event) override;

    private:
        QPointer<QLabel> createLabel(const QString &text);

        Game* game;

        QPointer<WindowBoard> dropZone;

        QPointer<QLabel> nextPieceLabel;
        QPointer<QLCDNumber> scoreLcd;
        QPointer<QLCDNumber> levelLcd;
        QPointer<QLCDNumber> hiLcd;
        QPointer<QPushButton> restartButton;
        QPointer<QPushButton> quitButton;
};

#endif