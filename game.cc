#include "game.h"
using namespace std;

Game::Game(bool isTextMode, int level, istream& is): in{is}, game{make_unique<Board>(level)}{}

void Game::parseCommand(const std::string& cmd) {
	
}
