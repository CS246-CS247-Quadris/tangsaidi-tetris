#include <sstream>
#include <iomanip>
#include "board.h"
using namespace std;

Board::Board(int level, const string& script): 
	curLevel{level}, board{15}, score{make_shared<Score>()} {
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
		if(i.first<0 || i.first>10) return false;
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

void Board::printRow(int y) {
	for(int x=0;x<11;x++) {
		vector<pair<int,int>> comp = cur->getComponents();
		bool ifHit=false;
		for(auto& v:comp) {
			if(v.first == x && v.second == y) {
				cout<<cur->getBlockType();
				ifHit=true;
			}
		}
		if(!ifHit)
			cout<<' ';
	}
}

void Board::print() {
	cout<<setw(10)<<left<<"Level:";
	cout<<setw(3)<<right<<curLevel<<endl;
	cout<<setw(10)<<left<<"Score:";
	cout<<setw(3)<<right<<score->getCurrentScore()<<endl;
	cout<<setw(10)<<left<<"Hi Score:";
	cout<<setw(3)<<right<<score->getHighestScore()<<endl;
	cout<<"-----------"<<endl;
	// First, check first 3 reserved row
	int nRows = board.size();
	for(int y=nRows+2;y>=nRows;y--) {
		printRow(y);
		cout<<endl;
	}
	for(int y=nRows-1;y>=0;y--) {
		stringstream ss;
		string tmp;
		ss<<board.at(nRows-y-1);
		tmp = ss.str();
		
		printRow(y);
		cout<<endl;
		
		ss.str(string());
	}
	cout<<"-----------"<<endl;
	
	cout<<"Next:"<<endl;
	// TODO
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

bool Board::checkEnd() {
	return !isValid(cur->getComponents());
}