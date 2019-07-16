#include <QtWidgets>

#include "window.h"

Window::Window(Game* game): game(game) {
    // Drop zone
    dropZone = new QFrame();
    dropZone->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    dropZone->setFocusPolicy(Qt::StrongFocus);

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

    // Hi score
    hiLcd = new QLCDNumber(5);
    hiLcd->setSegmentStyle(QLCDNumber::Filled);

    // Restart button
    restartButton = new QPushButton(tr("&Restart"));
    restartButton->setFocusPolicy(Qt::NoFocus);

    // Quit button
    quitButton = new QPushButton(tr("&Quit"));

    // std::unique_ptr<QGridLayout> layout = std::make_unique<QGridLayout>(new QGridLayout);
    QPointer<QGridLayout> layout = new QGridLayout;

    layout->addWidget(createLabel(tr("NEXT")).release(), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(createLabel(tr("LEVEL")).release(), 2, 0);
    layout->addWidget(levelLcd, 3, 0);
    layout->addWidget(dropZone, 0, 1, 12, 2);
    layout->addWidget(createLabel(tr("SCORE")).release(), 0, 3);
    layout->addWidget(scoreLcd, 1, 3);
    layout->addWidget(createLabel(tr("HI SCORE")).release(), 2, 3);
    layout->addWidget(hiLcd, 1, 3);
    layout->addWidget(restartButton, 4, 3);
    layout->addWidget(quitButton, 5, 3);

    setLayout(layout);
    // setLayout(layout.release());

    setWindowTitle(tr("Quadris"));
    resize(550, 600);
}

std::unique_ptr<QLabel> Window::createLabel(const QString &text) {
    std::unique_ptr<QLabel> lbl =  std::make_unique<QLabel>(new QLabel(text));
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}
