#include "levelZeroStrategy.h"
#include <fstream>

LevelZeroStrategy::LevelZeroStrategy() {
  setSequence();
}


void LevelZeroStrategy::setSequence(std::string fileName) {
  std::ifstream inFile;
  inFile.open(fileName);
  if (!inFile) {
    std::cerr << "Failed to open file" << std::endl;
    return;
  }

  char c;
  sequence = std::vector<char>();
  while (inFile >> c) {
    sequence.push_back(c);
  }
  inFile.close();
}


// get the next block on this level
std::unique_ptr<Block> LevelZeroStrategy::getNext() {
  if (current == sequence.size()) {
    current = 0;
  }
  return Block.create(sequence[current++]);
}

// Change the location of current block by calling the move function of block
// params: direction(n, s, e, w), distance(>=0)
void LevelZeroStrategy::move(char direction, int steps) {
  board.cur->move(direction, steps);
}

// Perform a rotation by calling the current block's rotate function
// params: isClockWise(true, false)
void LevelZeroStrategy::rotate(bool isClockwise) {
  board.cur->rotate(isClockwise);
}