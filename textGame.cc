#include "textGame.h"

#include <sstream>

TextGame::TextGame(Game* game): game(game) {
    game->printBoard();
    std::cout<<"> "<< std::flush;
    m_notifier = std::make_unique<QSocketNotifier>(fileno(stdin), QSocketNotifier::Read, this);
}

void TextGame::run() {
    connect(&(*m_notifier), SIGNAL(activated(int)), this, SLOT(readCommand()));
}

void TextGame::readCommand(std::istream& in) {
	if (!game->parseCommand(in)) {
		QCoreApplication::quit();
	}
    game->printBoard();
    emit updateWindow();
	std::cout<<"> "<< std::flush;
}


void TextGame::execCommand(std::string command) {
    std::istringstream iss(command);
    readCommand(iss);
}