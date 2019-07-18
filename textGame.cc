#include "textGame.h"

#include <sstream>

TextGame::TextGame(Game* game): game(game) {
    game->printBoard();
    std::cout<<"> "<< std::flush;
    m_notifier = std::make_unique<QSocketNotifier>(fileno(stdin),
        QSocketNotifier::Read, this);
}

void TextGame::run() {
    connect(&(*m_notifier), SIGNAL(activated(int)), this, SLOT(readCommand()));
}

void TextGame::readCommand(std::istream& in) {
    // Read until parseCommand returns false
    // Note, it only returns false when it needs to exit
	if (!game->parseCommand(in)) {
		QCoreApplication::quit();
	}
    game->printBoard();
    emit updateWindow();
	std::cout<<"> "<< std::flush;
}


void TextGame::execCommand(std::string command) {
    // put the command into an istringstream so readCommand can read from it
    std::istringstream iss(command);
    readCommand(iss);
}