
#include <QtWidgets>

#include "tetrixboard.h"
#include "tetrixwindow.h"

//! [0]
TetrixWindow::TetrixWindow()
{
    board = new TetrixBoard;
//! [0]

    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    board->setNextPieceLabel(nextPieceLabel);

//! [1]
    scoreLcd = new QLCDNumber(5);
    scoreLcd->setSegmentStyle(QLCDNumber::Filled);
//! [1]
    levelLcd = new QLCDNumber(2);
    levelLcd->setSegmentStyle(QLCDNumber::Filled);
    linesLcd = new QLCDNumber(5);
    linesLcd->setSegmentStyle(QLCDNumber::Filled);

//! [2]
    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));

//! [3] //! [4]

    connect(startButton, SIGNAL(clicked()), board, SLOT(start()));
// //! [4] //! [5]
    connect(quitButton , SIGNAL(clicked()), qApp, SLOT(quit()));
//     connect(pauseButton, SIGNAL(clicked()), board, SLOT(pause()));
//     connect(board, SIGNAL(scoreChanged(int)), scoreLcd, SLOT(display(int)));
//     connect(board, SIGNAL(levelChanged(int)), levelLcd, SLOT(display(int)));
//     connect(board, SIGNAL(linesRemovedChanged(int)),
//             linesLcd, SLOT(display(int)));
//! [5]

//! [6]
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel(tr("NEXT")), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(createLabel(tr("LEVEL")), 2, 0);
    layout->addWidget(levelLcd, 3, 0);
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(board, 0, 1, 12, 2);
    layout->addWidget(createLabel(tr("SCORE")), 0, 3);
    layout->addWidget(scoreLcd, 1, 3);
    layout->addWidget(quitButton, 4, 3);
    setLayout(layout);

    setWindowTitle(tr("Tetrix"));
    resize(550, 600);
}
//! [6]

//! [7]
QLabel *TetrixWindow::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}
//! [7]

