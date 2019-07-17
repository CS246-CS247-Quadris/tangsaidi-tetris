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
    // window does not own the game
    
    public:
        Window(Game*);

    signals:
        void execCommand(std::string command);
    
    public slots:
		void updateWindow();

    private slots:
        void handleRestart();

    protected:
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