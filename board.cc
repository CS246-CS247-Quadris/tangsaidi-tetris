#include "board.h"
using namespace std;

Board::Board(int level, const string& script): curLevel{level}, board{15} {
	strategy = Level::create(curLevel, this);
	if(!script.empty())
		strategy->setScriptFile(script);
	// need to check off by one case here for level 4
	cur = strategy->getNext();
	next= strategy->getNext();
}

int Board::getCurrentLevel() const {
	return curLevel;
}

bool Board::isValid(std::vector<std::pair<int, int>> coord) {
	for (auto &i : coord) {
		if (i.second >= board.size() || i.second < 0) return false;
		if (board.at(i.second).isOccupied(i.first)) return false;
	}
	return true;
}

void Board::move(char direction, int steps) {
	// TODO: check level and strategy
	strategy->move(direction, steps);
	if(!isValid(cur->getComponents()))
		strategy->move(direction, -steps);
}

void Board::rotate(bool isClockWise) {
	// TODO: check level and strategy
	strategy->rotate(isClockWise);
	if(!isValid(cur->getComponents()))
		strategy->rotate(!isClockWise);
}

void Board::changeLevel(int delta) {
	int dest = curLevel+delta;
	if(dest>=0 && dest<=4) {
		curLevel = dest;
		strategy = Level::create(curLevel, this);
	}
}

void Board::print() {
	for(auto v:board) {
		cout<<v<<endl;
	}
}

void Board::norand(bool isNoRandom, string file) {
	strategy->setNorandom(isNoRandom);
	if(isNoRandom)
		strategy->setSequence(file);
}

void Board::setSeed(int seed) {
	strategy->setSeed(seed);
}

void Board::createSettler(std::pair<int, int> coord) {
	//did not perform unit test for this method, to be tested
	shared_ptr<Settler> s = make_shared<Settler>(-1, score);
	board.at(coord.second).setRowAt(coord.first, '*', s);
}

void Board::createSettler(std::vector<std::pair<int, int>> coord, char blockType, int blockLevel) {
	shared_ptr<Settler> s = make_shared<Settler>(blockLevel, score);
	for (auto &i : coord) {
		board.at(i.second).setRowAt(i.first, blockType, s);
	}
}

void Board::hint(){
	//penalize height, holes, blockade(blocks directly above holes)
	//reward clears

	//Step1: look at all possible combinations of current and next block (stage1: current only)
	//Step2: obtain a score for each of the position
	//Step3: display the pisition with the highest score (TODO: if create settler, how to delete?)
}

// **TEST REQUIRED**
void Board::drop() {
	while (isValid(cur->getComponents())) {
		strategy->move('d', 1);
	}
	strategy->move('d', -1);
	createSettler(cur->getComponents(), cur->getBlockType(), cur->getBlockLevel());
	cur = std::move(next);
	next= strategy->getNext();
	vector<int> toBeRemoved;
	for (int i = 0; i < board.size(); ++i) {
		if (board[i].isRemovable()) {
			toBeRemoved.push_back(i);
		}
	}

	for (auto it = toBeRemoved.rbegin(); it != toBeRemoved.rend(); ++it) {
		board.push_back(Row());
		board.erase(board.begin(), board.begin() + *it);
	}
}
