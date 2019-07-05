#include "game.h"
using namespace std;

Game::Game() : game{make_unique<Board>()}{}

void Game::parseCommand(const std::string& cmd) {
	
}
