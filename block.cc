#include "block.h"
#include <iostream>

//std::unique_ptr<Block> Block::create(char type) {
//	std::cout << "Block " << type << " created" << std::endl;
//	return std::make_unique<Block>();
//}

Block::Block(char t, Board& b):type{t},board{b}{}

/* Block::move
 *
 * direction: 'l' for left, 'r' for right, 'd' for down
 */
void Block::move(char direction, int steps) {
	std::cout << "Moved direction: " << direction << "  distance: " << steps << std::endl;
	switch(direction) {
		case 'l':
			break;
		case 'r':
			break;
		case 'd':
			break;
	}
}

void Block::rotate(bool isClockwise) {
	std::cout << "Rotated clockwise: " << isClockwise << std::endl;
}

//IBlock::IBlock():Block('I')
