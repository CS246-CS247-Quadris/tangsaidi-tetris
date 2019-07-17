#include <QtWidgets>

#include "window.h"

Window::Window(Game* game): game(game) {
    // Drop zone
    dropZone = new WindowBoard(0, game);

    // Next piece of block
    nextPieceLabel = new QLabel();
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);

    // Current score
    scoreLcd = new QLCDNumber(5);
    scoreLcd->setSegmentStyle(QLCDNumber::Filled);

    // Current Level
    levelLcd = new QLCDNumber(2);
    levelLcd->setSegmentStyle(QLCDNumber::Filled);
    levelLcd->display(game->game->getCurrentLevel());

    // Hi score
    hiLcd = new QLCDNumber(5);
    hiLcd->setSegmentStyle(QLCDNumber::Filled);

    // Restart button
    restartButton = new QPushButton(tr("&Restart"));
    restartButton->setFocusPolicy(Qt::NoFocus);

    // Quit button
    quitButton = new QPushButton(tr("&Quit"));

    connect(quitButton , SIGNAL(clicked()), qApp, SLOT(quit()));


    QPointer<QGridLayout> layout = new QGridLayout;

    layout->addWidget(createLabel(tr("NEXT")), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(createLabel(tr("LEVEL")), 2, 0);
    layout->addWidget(levelLcd, 3, 0);
    
    layout->addWidget(dropZone, 0, 1, 12, 2);
    
    layout->addWidget(createLabel(tr("SCORE")), 0, 3);
    layout->addWidget(scoreLcd, 1, 3);

    layout->addWidget(createLabel(tr("HI SCORE")), 2, 3);
    layout->addWidget(hiLcd, 3, 3);
    
    layout->addWidget(restartButton, 4, 3);
    layout->addWidget(quitButton, 5, 3);

    setLayout(layout);

    setWindowTitle(tr("Quadris"));
    resize(800, 600);
}
QPointer<QLabel> Window::createLabel(const QString &text) {
    QPointer<QLabel> lbl =  new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}

void Window::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    levelLcd->display(game->game->getCurrentLevel());
    scoreLcd->display(game->game->getScore());
    hiLcd->display(game->game->getHiScore());

}

void Window::updateWindow() {
    this->repaint();
}
