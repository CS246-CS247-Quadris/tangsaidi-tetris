#include "block.h"
#include <iostream>
using namespace std;

//std::unique_ptr<Block> Block::create(char type) {
//	std::cout << "Block " << type << " created" << std::endl;
//	return std::make_unique<Block>();
//}

Block::Block(char t, Board& b):type{t},board{b}{}
Block::~Block(){}

std::vector<pair<int,int>> Block::getComponents() const {
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
IBlock::IBlock(Board& b):Block('I',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{2,14},{3,14}};
}

JBlock::JBlock(Board& b):Block('J',b) {
	coordinate = vector<pair<int,int>>{{0,14},{0,13},{1,13},{2,13}};
}

LBlock::LBlock(Board& b):Block('L',b) {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{2,13},{2,14}};
}

OBlock::OBlock(Board& b):Block('O',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{0,13},{1,13}};
}

SBlock::SBlock(Board& b):Block('S',b) {
	coordinate = vector<pair<int,int>>{{0,13},{1,13},{1,14},{2,14}};
}

ZBlock::ZBlock(Board& b):Block('Z',b) {
	coordinate = vector<pair<int,int>>{{0,14},{1,14},{1,13},{2,13}};
}

TBlock::TBlock(Board& b):Block('T',b) {
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
	if(coordinate[0].first != coordinate[1].second) {
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
