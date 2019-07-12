#include "board.h"
using namespace std;

const char Board::level1DistrTable[12] = {'S', 'Z', 'I', 'I', 'J', 'J', 'L', 'L', 'O', 'O', 'T', 'T'};

Board::Board(int level): curLevel{level} {
	if(level == 0)
		fin.open("sequence.txt", ifstream::in);
}

void Board::sequenceFile(const string& name) {
	if(fin.is_open())
		fin.close();
	fin.clear();
	fin.open(name.c_str(), ifstream::in);
}

int Board::getCurrentLevel() const {
	return curLevel;
}

void Board::move(char direction, int steps) {
	// TODO: check level and strategy
	cur->move(direction, steps);
}

void Board::rotate(bool isClockWise) {
	// TODO: check level and strategy
	cur->rotate(isClockWise);
}

void Board::changeLevel(int delta) {
	int dest = curLevel+delta;
	if(dest>=0 && dest<=4)
		curLevel = dest;
}
