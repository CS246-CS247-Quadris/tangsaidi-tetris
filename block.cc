#include "block.h"
#include <iostream>
using namespace std;

unique_ptr<Block> Block::create(char type, const shared_ptr<Board>& b) {
	cout << "Block " << type << " created" << endl;
	switch(type) {
		case 'I':
			return make_unique<IBlock>(b);
		case 'J':
			return make_unique<JBlock>(b);
		case 'L':
			return make_unique<LBlock>(b);
		case 'O':
			return make_unique<OBlock>(b);
		case 'S':
			return make_unique<SBlock>(b);
		case 'Z':
			return make_unique<ZBlock>(b);
		case 'T':
			return make_unique<TBlock>(b);
		default:
			return nullptr;
	}
}

Block::Block(char t, const shared_ptr<Board>& b):type{t},board{b}{}
Block::~Block(){}

Block& Block::operator=(const Block& other) {
	if(this != &other) {
		type = other.type;
		board = other.board;
		level = other.level;
	}
	return *this;
}

std::vector<pair<int,int>> Block::getComponents() const {
	return coordinate;
}

/* Block::move
 *
 * direction: 'l' for left, 'r' for right, 'd' for down
 *
 * assumption: the coordinate system of the board has origin at bottom-left corner
 */
void Block::move(char direction, unsigned int steps) {
	cout << "Moved direction: " << direction << "  distance: " << steps << endl;
	switch(direction) {
		case 'l':
			for(auto& v:coordinate) {
				v.first -= steps;
			}
			break;
		case 'r':
			for(auto& v:coordinate) {
				v.first += steps;
			}
			break;
		case 'd':
			for(auto& v:coordinate) {
				v.second -= steps;
			}
			break;
	}
}

void Block::rotate(bool isClockwise) {
	cout << "Rotated clockwise: " << isClockwise << endl;
	int x=coordinate[0].first, y=coordinate[0].second;
	for(auto& v:coordinate) {
		// since the size of bounding box is known, so min is enough
		if(v.first < x) {
			x = v.first;
		}
		if(v.second < y) {
			y = v.second;
		}
	}
	// find center of 3x3 bounding box
	// assumption: x,y always positive
	x += 1;
	y += 1;
	// transform coordinates
	for(auto& v:coordinate) {
		v.first -= x;
		v.second -= y;
	}
	// perform rotation
	// clockwise: x->y, y->-x
	if(isClockwise) {
		for(auto& v:coordinate) {
			int tmp = v.first;
			v.first = v.second;
			v.second = -tmp;
		}
	}
	// counterclockwise: x->-y, y->x
	else {
		for(auto& v:coordinate) {
			int tmp = v.first;
			v.first = -v.second;
			v.second = tmp;
		}
	}
	// transform coordinates
	for(auto& v:coordinate) {
		v.first += x;
		v.second += y;
	}
}

// 11 col, 15 row
IBlock::IBlock(const shared_ptr<Board>& b):Block('I',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{2,14},{3,14}};
}

JBlock::JBlock(const shared_ptr<Board>& b):Block('J',b) {
	coordinate = vector<pair<int,int>>{{0,14},{0,13},{1,13},{2,13}};
}

LBlock::LBlock(const shared_ptr<Board>& b):Block('L',b) {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{2,13},{2,14}};
}

OBlock::OBlock(const shared_ptr<Board>& b):Block('O',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{0,13},{1,13}};
}

SBlock::SBlock(const shared_ptr<Board>& b):Block('S',b) {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{1,14},{2,14}};
}

ZBlock::ZBlock(const shared_ptr<Board>& b):Block('Z',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{1,13},{2,13}};
}

TBlock::TBlock(const shared_ptr<Board>& b):Block('T',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{2,14},{1,13}};
}

void IBlock::rotate(bool isClockwise) {
	int x=coordinate[0].first, y=coordinate[0].second;
	for(auto& v:coordinate) {
		if(v.first < x) {
			x = v.first;
		}
		if(v.second < y) {
			y = v.second;
		}
	}
	
	// if horizontal, rotate it to vertical
	if(coordinate[0].second == coordinate[1].second) {
		for(int i=0;i<coordinate.size();i++) {
			coordinate[i].first = x;
			coordinate[i].second = y+i;
		}
	}
	// rotate it to horizontal otherwise
	else {
		for(int i=0;i<coordinate.size();i++) {
			coordinate[i].first = x+i;
			coordinate[i].second = y;
		}
	}
}

// O block remains unchanged after any rotation
void OBlock::rotate(bool isClockwise) {}
