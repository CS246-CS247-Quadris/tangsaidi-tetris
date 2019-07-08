#include "block.h"
#include <iostream>

//std::unique_ptr<Block> Block::create(char type) {
//	std::cout << "Block " << type << " created" << std::endl;
//	return std::make_unique<Block>();
//}

Block::Block(char t, Board& b):type{t},board{b}{}
Block::~Block(){}

std::vector<std::pair<int,int>> Block::getComponents() const {
	return coordinate;
}

/* Block::move
 *
 * direction: 'l' for left, 'r' for right, 'd' for down
 *
 * assumption: the coordinate system of the board has origin at bottom-left corner
 */
void Block::move(char direction, int steps) {
	std::cout << "Moved direction: " << direction << "  distance: " << steps << std::endl;
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
	std::cout << "Rotated clockwise: " << isClockwise << std::endl;
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

IBlock::IBlock(Board& b):Block('I',b){}
JBlock::JBlock(Board& b):Block('J',b){}
LBlock::LBlock(Board& b):Block('L',b){}
OBlock::OBlock(Board& b):Block('O',b){}
SBlock::SBlock(Board& b):Block('S',b){}
ZBlock::ZBlock(Board& b):Block('Z',b){}
TBlock::TBlock(Board& b):Block('T',b){}

void IBlock::rotate(bool isClockwise) {
	
}

void OBlock::rotate(bool isClockwise) {}
