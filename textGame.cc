#include "textGame.h"

#include <iostream>

TextGame::TextGame(Game* game): game(game) {
    game->printBoard();
    std::cout<<"> "<< std::flush;
    m_notifier = std::make_unique<QSocketNotifier>(fileno(stdin), QSocketNotifier::Read, this);
}

void TextGame::run() {
    connect(&(*m_notifier), SIGNAL(activated(int)), this, SLOT(readCommand()));
}

void TextGame::readCommand() {
	if (!game->parseCommand(std::cin)) {
        std::cout << "Game over" << std::endl;
		QApplication::quit();
	}
    game->printBoard();
    emit updateWindow();
	std::cout<<"> "<< std::flush;
}