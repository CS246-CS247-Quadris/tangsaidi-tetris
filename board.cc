#include "board.h"
using namespace std;

Board::Board(int level): curLevel{level}{}

int Board::getCurrentLevel() const {
	return curLevel;
}
