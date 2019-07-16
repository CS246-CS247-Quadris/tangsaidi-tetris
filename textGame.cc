#include "textGame.h"

#include <iostream>

TextGame::TextGame(Game* game): game(game) {
    m_notifier = std::make_unique<QSocketNotifier>(fileno(stdin), QSocketNotifier::Read, this);
}

void TextGame::run() {
    connect(&(*m_notifier), SIGNAL(activated(int)), this, SLOT(readCommand()));
}

void TextGame::readCommand() {
	game->printBoard();
	std::cout<<"> ";
	if (!game->parseCommand(std::cin)) {
		QApplication::quit();
	}
}
