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
		if (i.second < board.size() && board.at(i.second).isOccupied(i.first) && board.at(i.second).getData(i.first) != '?') return false;
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

std::stringstream Board::printRow(int y) {
	std::stringstream ss;
	for(int x=0;x<11;x++) {
		vector<pair<int,int>> comp = cur->getComponents();
		bool ifHit = false;
		for(auto& v:comp) {
			if(v.first == x && v.second == y && !checkEnd()) {
				if (y >= 15 || !board.at(y).isOccupied(x)) {
					ss << cur->getBlockType();
					ifHit = true;
				}
			}
		}
		if (y < 15 && board.at(y).isOccupied(x)) {
			ss << board.at(y).getData(x);
			ifHit = true;
		}
		if(!ifHit) {
			ss <<' ';
		}
	}
	return ss;
}

std::stringstream Board::toStringStream() {
	std::stringstream ss;
	int nRows = board.size();
	for(int y=nRows+2;y>=0;y--) {
		ss << printRow(y).str() << "\n";
	}
	return ss;
}

std::string Board::getStringified() const {
	return stringified;
}

std::string Board::getStringifiedNext() const {
	switch(next->getBlockType()) {
		case 'I':
			return "IIII";
		case 'J':
			return "J\nJJJ";
		case 'L':
			return "  L\nLLL";
		case 'O':
			return "OO\nOO";
		case 'S':
			return" SS\nSS";
		case 'Z':
			return "ZZ\n ZZ";
		case 'T':
			return "TTT\n T";
		default:
			cerr<<"Failed reading shape of next block."<<endl;
			return "?";
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
	stringified = toStringStream().str();
	cout << stringified;

	cout<<"-----------"<<endl;
	
	cout<<"Next: "<<endl;
	if(next == nullptr) {
		cout<<"  N/A"<<endl;
		return;
	}
	cout << getStringifiedNext() << endl;
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

void Board::createHintSettler(std::vector<std::pair<int, int>> coord) {
	shared_ptr<Settler> s = make_shared<Settler>(-1, score);
	hintSettlerCoord = coord;
	for (auto &i : coord) {
		board.at(i.second).setRowAt(i.first, '?', s);
	}
}

void Board::deleteHintSettler() {
	for (auto &i : hintSettlerCoord) {
		board.at(i.second).clearPixelAt(i.first);
	}
	hintSettlerCoord.clear();
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

bool Board::isHole(int row, int right, int left) {
	if (row + 1 >= hintBoard.size()) return false;
	for (int i = right; i <= left; ++i) {
		if (hintBoard.at(row+1).at(i) == false)
			return false;
	}
	return true;
}

bool Board::isHalfHole(int y, int x) {
	if (y - 1 < 0) return false;
	if (hintBoard.at(y-1).at(x) == true)
		return false;
	if (x - 1 < 0 || hintBoard.at(y-1).at(x-1) == true)
		return true;
	if (x + 1 >= 11 || hintBoard.at(y-1).at(x+1) == true)
		return true;
	return false;
}

pair<int, int> Board::findHoles() {
	int numOfHoles = 0;
	int halfHoles = 0;

	for (int i = 0; i < hintBoard.size(); ++i) {
		for (int j = 0; j < 11; ++j) {
			if (hintBoard.at(i).at(j) == false) {
				int right = j;
				while (j + 1 < 11 && hintBoard.at(i).at(j + 1) == false) {
					j++;
				}
				if (isHole(i, right, j)) {
					numOfHoles += j - right + 1;
				}
			}
		}
	}

	for (int i = 0; i < hintBoard.size(); ++i) {
		for (int j = 0; j < 11; ++j) {
			if (hintBoard.at(i).at(j) == true && isHalfHole(i, j)) {
				halfHoles ++;
				while (j + 1 < 11 && hintBoard.at(i).at(j + 1) == true) {
					halfHoles++;
					j++;
				}
			}
		}
	}
	return make_pair(numOfHoles, halfHoles);
}

pair<int, int> Board::findEdgesAndHeight() {
	int edgeNum = 0;
	int prevHeight = -1;
	int maxHeight = -1;
	for (int i = 0; i < 11; ++i) {
		int j = hintBoard.size() - 1;
		while (j >= 0 && hintBoard.at(j).at(i) == false) {
			j--;
		}
		if (maxHeight == -1) {
			maxHeight = j;
			prevHeight = j;
		} else if(maxHeight < j) {
			maxHeight = j;
		}
	}
	//find the total edge of the current shape.
	//search all 4*4, if three empty or one empty, central point is a vertex?
	for (int i = 0; i + 1 < 11; ++i) {
		for (int j = 0; j + 1 < hintBoard.size(); ++j) {
			int emptyPixel = 0;
			int specialCase1 = 0;
			int specialCase2 = 0;
			if (hintBoard.at(j).at(i) == false) {
				emptyPixel++;
				if (j == 0) specialCase2++;
			} else if (j == 0) {
				specialCase1 ++;
			}
			if (hintBoard.at(j + 1).at(i) == false) emptyPixel++;
			if (hintBoard.at(j).at(i + 1) == false) {
				emptyPixel++;
				if (j == 0) specialCase1 ++;
			} else if (j == 0) {
				specialCase2 ++;
			}
			if (hintBoard.at(j + 1).at(i + 1) == false) emptyPixel++;
			if (emptyPixel == 3 || emptyPixel == 1) edgeNum++;
			if (specialCase1 == 2 || specialCase2 == 2) edgeNum++; 
		}
	}
	return make_pair(maxHeight + 1, edgeNum + 1);
}

bool Board::hintIsRemovable(int i) {
	for (int j = 0; j < 11; ++j) {
		if (hintBoard.at(i).at(j) == false) return false;
	}
	return true;
}

int Board::checkAndRemoveRow() {
	vector<int> toBeRemoved;
	for (int i = 0; i < hintBoard.size(); ++i) {
		if (hintIsRemovable(i)) {
			toBeRemoved.emplace_back(i);
		}
	}
	for (auto it = toBeRemoved.rbegin(); it != toBeRemoved.rend(); ++it) {
		vector<bool> newRow(11, false);
		hintBoard.emplace_back(newRow);
		hintBoard.erase(hintBoard.begin() + *it);
	}
	return toBeRemoved.size();
}

vector<pair<int,int>> Board::singleOrientationHint() {
	vector<pair<int, int>> bestCoord;
	int bestScore = -1;
	pair<int, int> bestShape = make_pair(-1, -1);
	pair<int, int> bestHole = make_pair(-1, -1);
	if (curLevel >= 3) cur->move('d', 1);
	//try all possible positions as the block is moved to the left
	int i = 0;
	while(isValid(cur->getComponents())) {
		vector<pair<int, int>> tmpPos = ifDropNow(cur->getComponents());
		set<pair<int, int>> preserved;
		for (auto &i : tmpPos) {
			preserved.insert(i);
		}
		//for each possible position, set up the hintBoard to check simulated board after drop
		hintBoard.clear();
		vector<bool> tmpRow;
		for (int i = 0; i < board.size(); ++i) {
			tmpRow.clear();
			for (int j = 0; j < 11; ++j) {
				if (board.at(i).isOccupied(j) || preserved.find(make_pair(j, i)) != preserved.end())
					tmpRow.emplace_back(true);
				else 
					tmpRow.emplace_back(false);
			}
			hintBoard.emplace_back(tmpRow);
		}
		int removableRow = checkAndRemoveRow();
		pair<int, int> holes = findHoles();
		pair<int, int> shape = findEdgesAndHeight();
		//find the number of holes and max height with position after drop
		if (bestScore >= 0 && bestHole.first >= 0 && bestHole.second >= 0 && bestShape.first >= 0 && bestShape.second >= 0) {
			//compare the result with the best result from before, determine new best result
			if (removableRow > bestScore) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first < bestHole.first) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first == bestHole.first && holes.second < bestHole.second) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first == bestHole.first 
				&& holes.second == bestHole.second && shape.first < bestShape.first) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first == bestHole.first && holes.second == bestHole.second 
				&& shape.first == bestShape.first && shape.second < bestShape.second) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			}
		} else {
			//if not initialized, current result is the best
			bestScore = removableRow;
			bestHole = holes;
			bestShape = shape;
			bestCoord = tmpPos;
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
		//for each possible position, set up the hintBoard to check simulated board after drop
		hintBoard.clear();
		vector<bool> tmpRow;
		for (int i = 0; i < board.size(); ++i) {
			tmpRow.clear();
			for (int j = 0; j < 11; ++j) {
				if (board.at(i).isOccupied(j) || preserved.find(make_pair(j, i)) != preserved.end())
					tmpRow.emplace_back(true);
				else 
					tmpRow.emplace_back(false);
			}
			hintBoard.emplace_back(tmpRow);
		}
		int removableRow = checkAndRemoveRow();
		pair<int, int> holes = findHoles();
		pair<int, int> shape = findEdgesAndHeight();
		//find the number of holes and max height with position after drop
		if (bestScore >= 0 && bestHole.first >= 0 && bestHole.second >= 0 && bestShape.first >= 0 && bestShape.second >= 0) {
			//compare the result with the best result from before, determine new best result
			if (removableRow > bestScore) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first < bestHole.first) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first == bestHole.first && holes.second < bestHole.second) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first == bestHole.first 
				&& holes.second == bestHole.second && shape.first < bestShape.first) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			} else if (removableRow == bestScore && holes.first == bestHole.first && holes.second == bestHole.second 
				&& shape.first == bestShape.first && shape.second < bestShape.second) {
				bestScore = removableRow;
				bestHole = holes;
				bestShape = shape;
				bestCoord = tmpPos;
			}
		} else {
			//if not initialized, current result is the best
			bestScore = removableRow;
			bestHole = holes;
			bestShape = shape;
			bestCoord = tmpPos;
		}
		cur->move('r', 1);
		i++;
	}
	//revert back the original block
	cur->move('l', i);
	if (curLevel >= 3) cur->move('d', -1);
	//return results
	vector<pair<int, int>> result;
	result.emplace_back(make_pair(bestScore, 0));
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
	vector<pair<int, int>> hintResult = singleOrientationHint();
	cout << "rotation 0" << endl;
	cout << "removable row: " << hintResult.at(0).first << endl;
	cout << "holes: " << hintResult.at(1).first << " halfHoles: " << hintResult.at(1).second << endl;
	cout << "maxHeight: " << hintResult.at(2).first << " edges: " << hintResult.at(2).second << endl;	
	for (int i = 3; i < hintResult.size(); i++) {
		cout << "(" << hintResult.at(i).first << ", " << hintResult.at(i).second << ")" << endl;	
	}
	for (int i = 0; i < 3; ++i) {
		cur->rotate(true);
		vector<pair<int, int>> tmpBlock = singleOrientationHint();
		cout << "rotation " << i + 1 << endl;
		cout << "removable row: " << tmpBlock.at(0).first << endl;
		cout << "holes: " << tmpBlock.at(1).first << " halfHoles: " << tmpBlock.at(1).second << endl;
		cout << "maxHeight: " << tmpBlock.at(2).first << " edges: " << tmpBlock.at(2).second << endl;		
		for (int i = 3; i < tmpBlock.size(); i++) {
			cout << "(" << tmpBlock.at(i).first << ", " << tmpBlock.at(i).second << ")" << endl;	
		}
		if (tmpBlock.at(0).first > hintResult.at(0).first) {
			hintResult = tmpBlock;
		} else if (tmpBlock.at(0).first == hintResult.at(0).first && tmpBlock.at(1).first < hintResult.at(1).first) {
			hintResult = tmpBlock;
		} else if (tmpBlock.at(0).first == hintResult.at(0).first 
			&& tmpBlock.at(1).first == hintResult.at(1).first && tmpBlock.at(1).second < hintResult.at(1).second) {
			hintResult = tmpBlock;
		} else if (tmpBlock.at(0).first == hintResult.at(0).first && tmpBlock.at(1).first == hintResult.at(1).first 
			&& tmpBlock.at(1).second == hintResult.at(1).second && tmpBlock.at(2).first < hintResult.at(2).first) {
			hintResult = tmpBlock;
		} else if (tmpBlock.at(0).first == hintResult.at(0).first 
			&& tmpBlock.at(1).first == hintResult.at(1).first && tmpBlock.at(1).second == hintResult.at(1).second
			&& tmpBlock.at(2).first == hintResult.at(2).first && tmpBlock.at(2).second < hintResult.at(2).second) {
			hintResult = tmpBlock;
		}
	}
	cur->rotate(true);
	cout << "bestHint" << endl;
	cout << "removable row: " << hintResult.at(0).first << endl;
	cout << "holes: " << hintResult.at(1).first << " halfHoles: " << hintResult.at(1).second << endl;
	cout << "maxHeight: " << hintResult.at(2).first << " edges: " << hintResult.at(2).second << endl;
	if (hintResult.at(0).first < 0 && hintResult.at(1).first < 0 && hintResult.at(1).second < 0
		&& hintResult.at(2).first < 0 && hintResult.at(2).second < 0) {
		cout << "Invalid hint. Already at bottom." << endl;
	}
	hintResult.erase(hintResult.begin());
	hintResult.erase(hintResult.begin());
	hintResult.erase(hintResult.begin());
	//print board
	createHintSettler(hintResult);
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
	if (toBeRemoved.size() > 0) {
		int rowScore = pow((curLevel + toBeRemoved.size()), 2);
		score->increment(rowScore);
	}
	for (auto it = toBeRemoved.rbegin(); it != toBeRemoved.rend(); ++it) {
		board.push_back(Row());
		board.erase(board.begin() + *it);
	}
}

bool Board::checkEnd() {
	return !isValid(cur->getComponents());
}

int Board::getHiScore() const {
	return Score::getHighestScore();
}

int Board::getScore() const {
	return score->getCurrentScore();
}