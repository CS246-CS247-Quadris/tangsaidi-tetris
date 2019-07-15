#include "block.h"
#include <iostream>
using namespace std;

unique_ptr<Block> Block::create(char type, int level) {
	cout << "Block " << type << " created" << endl;
	switch(type) {
		case 'I':
			return make_unique<IBlock>(level);
		case 'J':
			return make_unique<JBlock>(level);
		case 'L':
			return make_unique<LBlock>(level);
		case 'O':
			return make_unique<OBlock>(level);
		case 'S':
			return make_unique<SBlock>(level);
		case 'Z':
			return make_unique<ZBlock>(level);
		case 'T':
			return make_unique<TBlock>(level);
		default:
			return nullptr;
	}
}

Block::Block(char t, int level):type{t}, rotCenter{1,14}, level{level} {}

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
IBlock::IBlock(int level):Block{'I', level} {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{2,14},{3,14}};
//	rotCenter = make_pair<int,int>(0,14);
}

JBlock::JBlock(int level):Block{'J', level} {
	coordinate = vector<pair<int,int>>{{0,14},{0,13},{1,13},{2,13}};
//	rotCenter = make_pair<int,int>(1,14);
}

LBlock::LBlock(int level):Block{'L', level} {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{2,13},{2,14}};
//	rotCenter = make_pair<int,int>(1,14);
}

OBlock::OBlock(int level):Block{'O', level} {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{0,13},{1,13}};
	// O block never use it
//	rotCenter = make_pair<int,int>(0,13);
}

SBlock::SBlock(int level):Block{'S', level} {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{1,14},{2,14}};
//	rotCenter = make_pair<int,int>(1,14);
}

ZBlock::ZBlock(int level):Block{'Z', level} {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{1,13},{2,13}};
//	rotCenter = make_pair<int,int>(1,14);
}

TBlock::TBlock(int level):Block{'T', level} {
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

Block::~Block() {}