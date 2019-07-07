#include "block.h"
#include <iostream>

std::unique_ptr<Block> Block::create(char type) {
  std::cout << "Block " << type << " created" << std::endl;
  return std::make_unique<Block>();
}

void Block::move(char direction, int steps) {
  std::cout << "Moved direction: " << direction << "  distance: " << steps << std::endl;
}

void Block::rotate(bool isClockwise) {
  std::cout << "Rotated clockwise: " << isClockwise << std::endl;
}