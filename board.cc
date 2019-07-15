#include <sstream>
#include <iomanip>
#include <set>
#include <cmath>
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

Board::~Board() {
	score->endGame();
}

int Board::getCurrentLevel() const {
	return curLevel;
}

bool Board::isValid(std::vector<std::pair<int, int>> coord) {
	for (auto &i : coord) {
		if(i.first<0 || i.first>10) return false;
		if (i.second > board.size()+2 || i.second < 0) return false;
		if (i.second < board.size() && board.at(i.second).isOccupied(i.first)) return false;
	}
	return true;
}

void Board::move(char direction, int steps) {
	strategy->move(direction, steps);
}

void Board::rotate(bool isClockWise) {
	strategy->rotate(isClockWise);
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
			if(v.first == x && v.second == y && !checkEnd()) {
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
	cout << "\033[2J\033[1;1H"; // clear screen
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
		printRow(y);
		cout<<endl;
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

// This may not be safe for higher levels
// **TEST REQUIRED**
void Board::replaceCurrentBlock(char cType) {
	if(cType == cur->getBlockType())
		return;
	
	int srcXmin, srcYmin, destXmin, destYmin;
	vector<std::pair<int,int>> comp = cur->getComponents();
	
	destXmin = comp.at(0).first;
	destYmin = comp.at(0).second;
	for(auto v:comp) {
		if(v.first < destXmin) {
			destXmin = v.first;
		}
		if(v.second < destYmin) {
			destYmin = v.second;
		}
	}
	
	cur = Block::create(cType, curLevel);
	comp = cur->getComponents();
	
	srcXmin = comp.at(0).first;
	srcYmin = comp.at(0).second;
	for(auto v:comp) {
		if(v.first < srcXmin) {
			srcXmin = v.first;
		}
		if(v.second < srcYmin) {
			srcYmin = v.second;
		}
	}
	
	// coordinates need to be fixed
	for(int c=destXmin-srcXmin;c>0;c--) cur->move('r', 1);
	while(!isValid(cur->getComponents())) cur->move('l', 1);
	for(int c=srcYmin-destYmin;c>0;c--) cur->move('d', 1);
	while(!isValid(cur->getComponents())) cur->move('d', -1);
}

void Board::createSettler(std::pair<int, int> coord) {
	//did not perform unit test for this method, to be tested
	shared_ptr<Settler> s = make_shared<Settler>(-1, score);
	board.at(coord.second).setRowAt(coord.first, '*', s);
}

void Board::createHintSettler(vector<pair<int, int>> coord) {
	shared_ptr<Settler> s = make_shared<Settler>(-1, score);
	for (auto &i : coord) {
		board.at(i.second).setRowAt(i.first, '?', s);
	}
}

void Board::deleteHintSettler(vector<pair<int, int>> coord) {
	for (auto &i : coord) {
		board.at(i.second).clearPixelAt(i.first);
	}
}

void Board::createSettler(std::vector<std::pair<int, int>> coord, char blockType, int blockLevel) {
	shared_ptr<Settler> s = make_shared<Settler>(blockLevel, score);
	for (auto &i : coord) {
		board.at(i.second).setRowAt(i.first, blockType, s);
	}
}

vector<pair<int, int>> Board::ifDropNow(const vector<pair<int,int>> & block) {
	vector<pair<int, int>> result = block;
	do {
		for (auto &i : result) {
			i.second -= 1;
		}
	} while (isValid(result));
	for (auto &i : result) {
		i.second += 1;
	}
	return result;
}

bool Board::isHole(int row, int right, int left, const set<pair<int, int>> & preserved) {
	// if (board.at(i).isOccupied(j) || preserved.find(make_pair(j, i)) != preserved.end()) return false;
	// if (j - 1 >= 0 && !board.at(i).isOccupied(j-1) && preserved.find(make_pair(j-1, i)) == preserved.end()) 
	// 	return false;
	// if (j + 1 < 11 && !board.at(i).isOccupied(j+1) && preserved.find(make_pair(j+1, i)) == preserved.end()) 
	// 	return false;
	// if (i + 1 < board.size() && !board.at(i+1).isOccupied(j) && preserved.find(make_pair(j, i+1)) == preserved.end()) 
	// 	return false;
	for (int i = right; i <= left; ++i) {
		if (row + 1 < board.size() && !board.at(row+1).isOccupied(i) && preserved.find(make_pair(i, row+1)) == preserved.end())
			return false;
	}
	return true;
}

bool Board::isHalfHole(int y, int x, const set<pair<int, int>> & preserved) {
	if (x - 1 < 0 || y - 1 < 0 || board.at(y-1).isOccupied(x-1) || preserved.find(make_pair(x-1, y-1)) != preserved.end())
		return true;
	if (x + 1 >= 11 || y - 1 < 0 || board.at(y-1).isOccupied(x+1) || preserved.find(make_pair(x+1, y-1)) != preserved.end())
		return true;
	return false;
}

pair<int, int> Board::findHoles(const set<pair<int, int>> & preserved) {
	int numOfHoles = 0;
	int halfHoles = 0;
	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < 11; ++j) {
			int right = j;
			while (j + 1 < 11 && !board.at(i).isOccupied(j+1) && preserved.find(make_pair(j+1, i)) == preserved.end()) {
				j++;
			}
			if (isHole(i, right, j, preserved)) {
				numOfHoles ++;
			}
		}
	}

	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < 11; ++j) {
			if (board.at(i).isOccupied(j) && isHalfHole(i, j, preserved)) {
				halfHoles ++;
			}
		}
	}
	return make_pair(numOfHoles, halfHoles);
}

pair<int, int> Board::findEdgesAndHeight(const set<pair<int, int>> & preserved) {
	int edgeNum = 1;
	int prevHeight = -1;
	int maxHeight = -1;
	for (int i = 0; i < 11; ++i) {
		int j = board.size() - 1;
		while (j >= 0 && !board.at(j).isOccupied(i) && preserved.find(make_pair(i, j)) == preserved.end()) {
			j--;
		}
		if (maxHeight == -1) {
			maxHeight = j;
			prevHeight = j;
		} else if(maxHeight < j) {
			maxHeight = j;
		}
	}
	//TODO: find the total edge of the current shape.
	//search all 4*4, if three empty then a vertex?
	for (int i = 0; i + 1 < 11; ++i) {
		for (int j = 0; j + 1 < board.size(); ++j) {
			int emptyPixel = 0;
			int sameLevelEmpty = 0;
			if (!board.at(j).isOccupied(i) && preserved.find(make_pair(i, j)) == preserved.end()) {
				emptyPixel++;
				sameLevelEmpty++;
			}
			if (!board.at(j + 1).isOccupied(i) && preserved.find(make_pair(i, j + 1)) == preserved.end()) emptyPixel++;
			if (!board.at(j).isOccupied(i + 1) && preserved.find(make_pair(i + 1, j)) == preserved.end()) {
				emptyPixel++;
				sameLevelEmpty++;
			}
			if (!board.at(j + 1).isOccupied(i + 1) && preserved.find(make_pair(i + 1, j + 1)) == preserved.end()) emptyPixel++;
			if (emptyPixel == 3 || emptyPixel == 1) edgeNum++;
			if (j == 0 && sameLevelEmpty == 1) edgeNum++;
		}
	}
	// pair<int, int> curPosition = make_pair(0, prevHeight);
	// char curDirection = 'l';
	// while (curPosition.second < 11) {
	// 	if (board.at(curPosition.second + 1).isOccupied(curPosition.first + 1)) edgeNum ++;
	// }
	
	return make_pair(edgeNum, maxHeight);
}

vector<pair<int,int>> Board::singleOrientationHint() {
	vector<pair<int, int>> bestCoord;
	pair<int, int> bestShape = make_pair(-1, -1);
	pair<int, int> bestHole = make_pair(-1, -1);
	//try all possible positions as the block is moved to the left
	int i = 0;
	while(isValid(cur->getComponents())) {
		vector<pair<int, int>> tmpPos = ifDropNow(cur->getComponents());
		set<pair<int, int>> preserved;
		for (auto &i : tmpPos) {
			preserved.insert(i);
		}
		pair<int, int> holes = findHoles(preserved);
		pair<int, int> shape = findEdgesAndHeight(preserved);
		//find the number of holes and max height with position after drop
		if (bestHole.first >= 0 && bestHole.second >= 0 && bestShape.first >= 0 && bestShape.second >= 0) {
			//compare the result with the best result from before, determine new best result
			if (holes.first < bestHole.first) {
				bestHole = holes;
				bestShape = shape;
			} else if (holes.first == bestHole.first && holes.second < bestHole.second) {
				bestHole = holes;
				bestShape = shape;
			} else if (holes.first == bestHole.first && holes.second == bestHole.second && shape.first < bestShape.first) {
				bestHole = holes;
				bestShape = shape;
			} else if (holes.first == bestHole.first && holes.second == bestHole.second 
				&& shape.first == bestShape.first && shape.second < bestShape.second) {
				bestHole = holes;
				bestShape = shape;
			}
		} else {
			//if not initialized, current result is the best
			bestHole = holes;
			bestShape = shape;
		}
		cur->move('l', 1);
		i++;
	}
	//revert cur to original state and continue to check the right side
	cur->move('r', i + 1);
	i = 1;
	while(isValid(cur->getComponents())) {
		vector<pair<int, int>> tmpPos = ifDropNow(cur->getComponents());
		set<pair<int, int>> preserved;
		for (auto &i : tmpPos) {
			preserved.insert(i);
		}
		pair<int, int> holes = findHoles(preserved);
		pair<int, int> shape = findEdgesAndHeight(preserved);
		//find the number of holes and max height with position after drop
		if (bestHole.first >= 0 && bestHole.second >= 0 && bestShape.first >= 0 && bestShape.second >= 0) {
			//compare the result with the best result from before, determine new best result
			if (holes.first < bestHole.first) {
				bestHole = holes;
				bestShape = shape;
			} else if (holes.first == bestHole.first && holes.second < bestHole.second) {
				bestHole = holes;
				bestShape = shape;
			} else if (holes.first == bestHole.first && holes.second == bestHole.second && shape.first < bestShape.first) {
				bestHole = holes;
				bestShape = shape;
			} else if (holes.first == bestHole.first && holes.second == bestHole.second 
				&& shape.first == bestShape.first && shape.second < bestShape.second) {
				bestHole = holes;
				bestShape = shape;
			}
		} else {
			//if not initialized, current result is the best
			bestHole = holes;
			bestShape = shape;
		}
		cur->move('r', 1);
		i++;
	}
	//revert back the original block
	cur->move('l', i);
	//return results
	vector<pair<int, int>> result;
	result.emplace_back(bestHole);
	result.emplace_back(bestShape);
	result.insert(result.end(), bestCoord.begin(), bestCoord.end());
	return result;
}

void Board::hint(){
	//find the number of holes that will be produced for each possible outsome
	//the solution wih least amount of holes win
	//if the same, then pick the one with the least max height
	//if same height, random for stage 1, and possibly look at next block for future use
	vector<pair<int, int>> hintBlock = singleOrientationHint();
	for (int i = 0; i < 3; ++i) {
		cur->rotate(true);
		vector<pair<int, int>> tmpBlock = singleOrientationHint();
		if (tmpBlock.at(0).first < hintBlock.at(0).first) {
			hintBlock = tmpBlock;
		} else if (tmpBlock.at(0).first == hintBlock.at(0).first && tmpBlock.at(0).second < hintBlock.at(0).second) {
			hintBlock = tmpBlock;
		} else if (tmpBlock.at(0).first == hintBlock.at(0).first && tmpBlock.at(0).second == hintBlock.at(0).second
			&& tmpBlock.at(1).first < hintBlock.at(1).first) {
			hintBlock = tmpBlock;
		} else if (tmpBlock.at(0).first == hintBlock.at(0).first && tmpBlock.at(0).second == hintBlock.at(0).second
			&& tmpBlock.at(1).first == hintBlock.at(1).first && tmpBlock.at(1).second < hintBlock.at(1).second) {
			hintBlock = tmpBlock;
		}
	}
	cur->rotate(true);
	hintBlock.erase(hintBlock.begin());
	hintBlock.erase(hintBlock.begin());
	//print board
	createHintSettler(hintBlock);
	print();
	deleteHintSettler(hintBlock);
}

/* Drops a block onto the board and turn it into a settler
 * Check whether there's removable rows, if there is, remove them
 */
void Board::drop() {
	for (int i = 0; i < 15; i++) {
		strategy->move('d', 1);
	}
	createSettler(cur->getComponents(), cur->getBlockType(), cur->getBlockLevel());
	cur = std::move(next);
	next= strategy->getNext();
	vector<int> toBeRemoved;
	for (int i = 0; i < board.size(); ++i) {
		if (board[i].isRemovable()) {
			toBeRemoved.push_back(i);
		}
	}
	int rowScore = pow((curLevel + toBeRemoved.size()), 2);
	score->increment(rowScore);
	for (auto it = toBeRemoved.rbegin(); it != toBeRemoved.rend(); ++it) {
		board.push_back(Row());
		board.erase(board.begin() + *it);
	}
}

bool Board::checkEnd() {
	return !isValid(cur->getComponents());
}
