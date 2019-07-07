#include "levelZero.h"
#include <fstream>
#include <iostream>

void LevelZero::setSequence(std::string fileName) {
  if (fileName != scriptFile) {
    std::cerr << "The scriptfile for level 0 cannot be changed" << std::endl;
  } else {
    Level::setSequence(fileName);
  }
}

LevelZero::LevelZero(std::shared_ptr<Board> board, int seed) {
  this->board = board;
  setSequence(scriptFile);
}

// get the next block on this level
std::unique_ptr<Block> LevelZero::getNext() {
  if (current == sequence.size()) {
    current = 0;
  }
  return Block::create(sequence[current++]);
}

// Change the location of current block by calling the move function of block
// params: direction(n, s, e, w), distance(>=0)
void LevelZero::move(char direction, int steps) {
  board->cur->move(direction, steps);
}

// Perform a rotation by calling the current block's rotate function
// params: isClockWise(true, false)
void LevelZero::rotate(bool isClockwise) {
  board->cur->rotate(isClockwise);
}

LevelZero::~LevelZero() {}