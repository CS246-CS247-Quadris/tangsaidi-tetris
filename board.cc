#include <sstream>
#include <iomanip>
#include <set>
#include <cmath>
#include "board.h"
using namespace std;

Board::Board(int level, const string& script): 
	score{make_shared<Score>()}, board{15}, curLevel{level} {
	if(!script.empty())
		Level::setScriptFile(script);
	strategy = Level::create(curLevel, this);
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
		if (i.second > (int) board.size()+2 || i.second < 0) return false;
		if (i.second < (int) board.size() && board.at(i.second).isOccupied(i.first) 
			&& board.at(i.second).getData(i.first) != '?') 
			return false;
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
	Level::setSeed(seed);
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

void Board::createHintSettler(const std::vector<std::pair<int, int>> & coord) {
	shared_ptr<Settler> s = make_shared<Settler>(-1, score);
	hintSettlerCoord = coord;
	for (auto &i : coord) {
		if (i.second >= (int) board.size() || i.first >= 11 || i.second < 0 || i.first < 0) {
			hintSettlerCoord.clear();
			return;
		}
		board.at(i.second).setRowAt(i.first, '?', s);
	}
}

void Board::deleteHintSettler() {
	if (hintSettlerCoord.size() == 0) return;
	for (auto &i : hintSettlerCoord) {
		board.at(i.second).clearPixelAt(i.first);
	}
	hintSettlerCoord.clear();
}

void Board::createSettler(const std::vector<std::pair<int, int>> & coord, char type, int level) {
	shared_ptr<Settler> s = make_shared<Settler>(level, score);
	for (auto &i : coord) {
		board.at(i.second).setRowAt(i.first, type, s);
	}
}

/* 	Board::ifDropNow
	Return the coordinates of passed in block if "drop" is performed.
 */
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

/* 	Board::isHole
	Return true if from right to left, it is a hole that can not be accessed.
	Otherwise false.
 */
bool Board::isHole(int row, int right, int left) {
	if (row + 1 >= (int) hintBoard.size()) return false;
	for (int i = right; i <= left; ++i) {
		if (hintBoard.at(row+1).at(i) == false)
			return false;
	}
	return true;
}

/* 	Board::countHalfHole 
	Delete hint settler created with hint function (stored in hintSettlerCoord)
	Should be called after display, before next command.
 */
int Board::countHalfHoles(int row, int right, int left) {
	if (row + 1 >= (int) hintBoard.size()) return 0;
	if (right == left) return false;
	if (right + 1 == left) {
		if (hintBoard.at(row+1).at(right) == true) return 1;
		else if (hintBoard.at(row+1).at(left) == true) return 1;
		else return 0;
	}

	int n = 0;
	int i = right;
	while(i < left && hintBoard.at(row+1).at(i) == true) {
		i++;
	}
	n += max(i - right, 0);
	int j = left;
	while(j > right && hintBoard.at(row+1).at(j) == true) {
		j--;
	}
	n += max(left - j, 0);
	return n;
}

/* 	Board::findHoles 
	Return a pair of integers, where first one is the total width of all holes,
	and the second one is the total width of half holes.
	Note: half holes is defined to be unoccupied pixels that can be accessed from only left or right.
 */
pair<int, int> Board::findHoles() {
	int numOfHoles = 0;
	int numofHalfHoles = 0;
	//half holes are the holes that are only blocked on one side.
	//From algorithm of hint, this case is not solvable so should be penilizaed.
	for (int i = 0; i < (int) hintBoard.size(); ++i) {
		for (int j = 0; j < 11; ++j) {
			if (hintBoard.at(i).at(j) == false) {
				int right = j;
				while (j + 1 < 11 && hintBoard.at(i).at(j + 1) == false) {
					j++;
				}
				if (isHole(i, right, j)) {
					numOfHoles += j - right + 1;
				} else {
					numofHalfHoles += countHalfHoles(i, right, j);
				}
			}
		}
	}
	//all number of holes are the width of the hole, not counting the depth
	return make_pair(numOfHoles, numofHalfHoles);
}

/* 	Board::findEdges 
	Return the total edge of the final shape on board.
 */
int Board::findEdges() {
	int edgeNum = 0;
	//search all 4*4, if three empty or one empty, central point is a vertex.
	for (int i = 0; i + 1 < 11; ++i) {
		for (int j = 0; j + 1 < (int) hintBoard.size(); ++j) {
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
	return edgeNum + 1;
}

/* 	Board::findHeight 
	Return a pair of integers where the first one is max height of all columns,
	and the second one is the total height of all columns.
 */
pair<int, int> Board::findHeight() {
	int maxHeight = -1;
	int sumHeight = 0;
	//find the maxHeight and total height
	for (int i = 0; i < 11; ++i) {
		int j = hintBoard.size() - 1;
		while (j >= 0 && hintBoard.at(j).at(i) == false) {
			j--;
		}
		sumHeight += (j + 1);
		if (maxHeight == -1) {
			maxHeight = j;
		} else if(maxHeight < j) {
			maxHeight = j;
		}
	}
	return make_pair(maxHeight + 1, sumHeight);
}

/* 	Board::hintIsRemovable 
	Return true if row i is removable on hintBoard.
 */
bool Board::hintIsRemovable(int i) {
	for (int j = 0; j < 11; ++j) {
		if (hintBoard.at(i).at(j) == false) return false;
	}
	return true;
}

/* 	Board::checkAndRemoveRow 
	Return the number of removable rows with all of them removed in hintBoard.
 */
int Board::checkAndRemoveRow() {
	vector<int> toBeRemoved;
	for (int i = 0; i < (int) hintBoard.size(); ++i) {
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

/* 	Board::getHintScore 
	Return a score for current hintBoard, with factors being 
	number of holes, half holes, maxHeight, totalHeight, and number of edges.
 */
int Board::getHintScore() {
	int numOfClear = checkAndRemoveRow();
	int numofEdges = findEdges();
	pair<int, int> holes = findHoles();
	pair<int, int> height = findHeight();
	//give curernt hint block a score, weight of each factors could be changed.
	int hintScore = -6 * numOfClear + numofEdges / 2 + 13 * holes.first + 11 * holes.second + 5 * height.first + 2 * height.second / 10;
	return hintScore;
}

/* 	Board::setHintBoard 
	Set up the hintBoard to check simulated board after drop
 */
void Board::setHintBoard(const vector<pair<int, int>> & block) {
	set<pair<int, int>> preserved;
	for (auto &i : block) {
		preserved.insert(i);
	}
	hintBoard.clear();
	vector<bool> tmpRow;
	for (int i = 0; i < (int) board.size(); ++i) {
		tmpRow.clear();
		for (int j = 0; j < 11; ++j) {
				if (board.at(i).isOccupied(j) || preserved.find(make_pair(j, i)) != preserved.end())
				tmpRow.emplace_back(true);
			else 
				tmpRow.emplace_back(false);
		}
		hintBoard.emplace_back(tmpRow);
	}
}

/* 	Board::singleOrientationHint
	Checks for all possible solutions within 1 move after rotation, return the best solution
 */
vector<pair<int,int>> Board::singleOrientationHint(bool rotated) {
	
	vector<pair<int, int>> bestCoord;
	pair<bool, int> bestScore = make_pair(false, 0);
	if (curLevel >= 3 && rotated == false) cur->move('d', 1);
	//try all possible positions as the block is moved to the left
	int i = 0;
	while(isValid(cur->getComponents())) {
		vector<pair<int, int>> tmpPos = ifDropNow(cur->getComponents());
		setHintBoard(tmpPos);
		int tmpScore = getHintScore();
		if (bestScore.first == false) {
			bestScore.second = tmpScore;
			bestScore.first = true;
			bestCoord = tmpPos;
		} else {
			if (bestScore.second > tmpScore) {
				bestScore.second = tmpScore;
				bestCoord = tmpPos;
			}
		}
		
		cur->move('l', 1);
		i++;
	}
	//revert cur to original state and continue to check the right side
	cur->move('r', i + 1);
	i = 1;
	while(isValid(cur->getComponents())) {
		vector<pair<int, int>> tmpPos = ifDropNow(cur->getComponents());
		setHintBoard(tmpPos);
		int tmpScore = getHintScore();
		if (bestScore.first == false) {
			bestScore.second = tmpScore;
			bestScore.first = true;
			bestCoord = tmpPos;
		} else {
			if (bestScore.second > tmpScore) {
				bestScore.second = tmpScore;
				bestCoord = tmpPos;
			}
		}
		cur->move('r', 1);
		i++;
	}
	//revert back the original block
	cur->move('l', i);
	if (curLevel >= 3 && rotated == false) cur->move('d', -1);
	//return results
	vector<pair<int, int>> result;
	result.emplace_back(make_pair(bestScore.second, 0));
	result.insert(result.end(), bestCoord.begin(), bestCoord.end());
	return result;
}

void Board::hint(){
	//check for all 4 rotations and get the best score
	vector<pair<int, int>> hintResult = singleOrientationHint(false);
	for (int i = 0; i < 3; ++i) {
		cur->rotate(true);
		vector<pair<int, int>> tmpBlock = singleOrientationHint(true);
		if (tmpBlock.at(0).first < hintResult.at(0).first) {
			hintResult = tmpBlock;
		}
	}
	cur->rotate(true);
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
	for (int i = 0; i < (int) board.size(); ++i) {
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