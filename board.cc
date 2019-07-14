#include <sstream>
#include <iomanip>
#include <set>
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
		bool ifHit = false;
		for(auto& v:comp) {
			if(v.first == x && v.second == y) {
				cout << cur->getBlockType();
				ifHit = true;
			}
		}
		if (y < 15 && board.at(y).isOccupied(x)) {
			cout << board.at(y).getData(x);
			ifHit = true;
		}
		if(!ifHit) {
			cout<<' ';
		}
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
	
	cout<<"Next: "<<endl;
	if(next == nullptr) {
		cout<<"  N/A"<<endl;
		return;
	}
	switch(next->getBlockType()) {
		case 'I':
			cout<<"\nIIII"<<endl;
			break;
		case 'J':
			cout<<"J\nJJJ"<<endl;
			break;
		case 'L':
			cout<<"  L\nLLL"<<endl;
			break;
		case 'O':
			cout<<"OO\nOO"<<endl;
			break;
		case 'S':
			cout<<" SS\nSS"<<endl;
			break;
		case 'Z':
			cout<<"ZZ\n ZZ"<<endl;
			break;
		case 'T':
			cout<<"TTT\n T"<<endl;
			break;
		default:
			cerr<<"Failed reading shape of next block."<<endl;
			break;
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

int Board::findHoles(const vector<pair<int,int>> & block) {
	int numOfHoles = 0;
	set<pair<int, int>> preserved;
	for (auto &i : block) {
		preserved.insert(i);
	}

	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < 11; ++j) {
			if (!board.at(i).isOccupied(j) 
				&& (board.at(i-1).isOccupied(j) || preserved.find(make_pair(i-1, j)) != preserved.end())
				&& (board.at(i+1).isOccupied(j) || preserved.find(make_pair(i+1, j)) != preserved.end()) 
				&& (board.at(i).isOccupied(j+1) || preserved.find(make_pair(i, j+1)) != preserved.end())) {
				numOfHoles ++;
			}
		}
	}
	return numOfHoles;
}

int Board::findMaxHeight(const vector<pair<int,int>> & block) {
	int maxHeight = -1;
	for (auto &i : block) {
		if (i.second > maxHeight) maxHeight = i.second;
	}
	int i = board.size() - 1;
	while (i > maxHeight && i >= 0) {
		for (int j = 0; j < 11; ++j) {
			if (board.at(i).isOccupied(j)) {
				maxHeight = i;
				j = 11;
			}
		}
		i--;
	}
	return maxHeight + 1;
}

vector<pair<int,int>> Board::singleOrientationHint(unique_ptr<Block> b) {
	vector<pair<int, int>> bestCoord;
	int bestHole = -1; 
	int bestHeight = -1;
	//TODO:
	// unique_ptr<Block> tmp = b;
	unique_ptr<Block> tmp;
	//try all possible positions as the block is moved to the left
	while(isValid(tmp->getComponents())) {
		vector<pair<int, int>> tmpPos = tmp->ifDropNow();
		int holes = findHoles(tmpPos);
		int maxHeight = findMaxHeight(tmpPos);
		//find the number of holes and max height with position after drop
		if (bestHole >= 0 && bestHeight >= 0) {
			//compare the result with the best result from before, determine new best result
			if (holes < bestHole) {
				bestCoord = tmpPos;
				bestHole = holes;
				bestHeight = maxHeight;
			} else if (holes == bestHole && maxHeight < bestHeight) {
				bestCoord = tmpPos;
				bestHole = holes;
				bestHeight = maxHeight;
			}
		} else {
			//if not initialized, current result is the best
			bestCoord = tmpPos;
			bestHole = holes;
			bestHeight = maxHeight;
		}
		tmp->move('l', 1);
	}
	//try all possible positions on the right
	//TODO:
	// tmp = b;
	tmp->move('r', 1);
	while(isValid(tmp->getComponents())) {
		vector<pair<int, int>> tmpPos = tmp->ifDropNow();
		int holes = findHoles(tmpPos);
		int maxHeight = findMaxHeight(tmpPos);
		//find the number of holes and max height with position after drop
		if (bestHole >= 0 && bestHeight >= 0) {
			//compare the result with the best result from before, determine new best result
			if (holes < bestHole) {
				bestCoord = tmpPos;
				bestHole = holes;
				bestHeight = maxHeight;
			} else if (holes == bestHole && maxHeight < bestHeight) {
				bestCoord = tmpPos;
				bestHole = holes;
				bestHeight = maxHeight;
			}
		} else {
			//if not initialized, current result is the best
			bestCoord = tmpPos;
			bestHole = holes;
			bestHeight = maxHeight;
		}
		tmp->move('r', 1);
	}
	vector<pair<int, int>> result;
	result.emplace_back(make_pair(bestHole, bestHeight));
	result.insert(result.end(), bestCoord.begin(), bestCoord.end());
	return result;
}

void Board::hint(){
	//penalize height, holes, blockade(blocks directly above holes)
	//reward clears

	//Step1: look at all possible combinations of current and next block (stage1: current only)
	//Step2: obtain a score for each of the position
	//Step3: display the pisition with the highest score (TODO: if create settler, how to delete?)

	//find the number of holes that will be produced for each possible outsome
	//the solution wih least amount of holes win
	//if the same, then pick the one with the least max height
	//if same height, random for stage 1, and possibly look at next block for future use
	// unique_ptr<Block> tmp = cur; to be added when look through all four rotations
	//TODO:
	// vector<pair<int, int>> hintBlock = singleOrientationHint(cur);
}

/* Drops a block onto the board and turn it into a settler
 * Check whether there's removable rows, if there is, remove them
 */
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
		board.erase(board.begin() + *it);
	}
}

bool Board::checkEnd() {
	return !isValid(cur->getComponents());
}
