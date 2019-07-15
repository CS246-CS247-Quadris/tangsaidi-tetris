#include "block.h"
#include <iostream>
using namespace std;

unique_ptr<Block> Block::create(char type) {
	cout << "Block " << type << " created" << endl;
	switch(type) {
		case 'I':
			return make_unique<IBlock>();
		case 'J':
			return make_unique<JBlock>();
		case 'L':
			return make_unique<LBlock>();
		case 'O':
			return make_unique<OBlock>();
		case 'S':
			return make_unique<SBlock>();
		case 'Z':
			return make_unique<ZBlock>();
		case 'T':
			return make_unique<TBlock>();
		default:
			return nullptr;
	}
}

Block::Block(char t):type{t}, rotCenter{1,14} {}
Block::~Block(){}
Block::Block(const Block& other): type{other.type}, level{other.level} {}

Block& Block::operator=(const Block& other) {
	if(this != &other) {
		type = other.type;
		level = other.level;
	}
	return *this;
}

vector<pair<int,int>> Block::getComponents() const {
	return coordinate;
}


/* Block::move
 *
 * direction: 'l' for left, 'r' for right, 'd' for down
 *
 * assumption: the coordinate system of the board has origin at bottom-left corner
 */
void Block::move(char direction, int steps) {
	cout << "Moved direction: " << direction << "  distance: " << steps << endl;
	switch(direction) {
		case 'l':
			for(auto& v:coordinate) {
				v.first -= steps;
			}
			rotCenter.first -= steps;
			break;
		case 'r':
			for(auto& v:coordinate) {
				v.first += steps;
			}
			rotCenter.first += steps;
			break;
		case 'd':
			for(auto& v:coordinate) {
				v.second -= steps;
			}
			rotCenter.second -= steps;
			break;
	}
}

void Block::rotate(bool isClockwise) {
	cout << "Rotated clockwise: " << isClockwise << endl;
	int x=rotCenter.first, y=rotCenter.second;
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
IBlock::IBlock():Block('I') {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{2,14},{3,14}};
//	rotCenter = make_pair<int,int>(0,14);
}

JBlock::JBlock():Block('J') {
	coordinate = vector<pair<int,int>>{{0,14},{0,13},{1,13},{2,13}};
//	rotCenter = make_pair<int,int>(1,14);
}

LBlock::LBlock():Block('L') {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{2,13},{2,14}};
//	rotCenter = make_pair<int,int>(1,14);
}

OBlock::OBlock():Block('O') {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{0,13},{1,13}};
	// O block never use it
//	rotCenter = make_pair<int,int>(0,13);
}

SBlock::SBlock():Block('S') {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{1,14},{2,14}};
//	rotCenter = make_pair<int,int>(1,14);
}

ZBlock::ZBlock():Block('Z') {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{1,13},{2,13}};
//	rotCenter = make_pair<int,int>(1,14);
}

TBlock::TBlock():Block('T') {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{2,14},{1,13}};
//	rotCenter = make_pair<int,int>(1,14);
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

// Get block types
char Block::getBlockType() {
	return type;
}

int Block::getBlockLevel() {
	return level;
}