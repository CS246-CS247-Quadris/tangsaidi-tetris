#include "levelZero.h"
#include <iostream>

void LevelZero::setSequence(std::string fileName) {
  if (fileName != scriptFile) {
    std::cerr << "The scriptfile for level 0 cannot be changed" << std::endl;
  } else {
    Level::setSequence(fileName);
  }
}

LevelZero::LevelZero(Board* board): Level(board) {
  setSequence(scriptFile);
}

// get the next block on this level
std::unique_ptr<Block> LevelZero::getNext() {
  if (current == sequence.size()) {
    current = 0;
  }
  return Block::create(sequence[current++], board->getCurrentLevel());
}

LevelZero::~LevelZero() {}