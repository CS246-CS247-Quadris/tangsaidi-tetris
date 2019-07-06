#include "levelTwoStrategy.h"
#include <stdlib>
#include <time>

// get the next block on this level
std::unique_ptr<Block> LevelTwoStrategy::getNext() {
  srand(time(null));
  int randNum = rand() % 7;
  char blockType = 'I';
  if (randNum < 1) {
    blockType = 'S';
  } else if (randNum < 2) {
    blockType = 'Z';
  } else if (randNum < 3) {
    blockType = 'I';
  } else if (randNum < 4) {
    blockType = 'J';
  } else if (randNum < 5) {
    blockType = 'L';
  } else if (randNum < 6) {
    blockType = 'O';
  } else if (randNum < 7) {
    blockType = 'T';
  }
  return Block.create(blockType);
}

// Change the location of current block by calling the move function of block
// params: direction(n, s, e, w), distance(>=0)
void LevelTwoStrategy::move(char direction, int steps) {
  board.cur->move(direction, steps);
}

// Perform a rotation by calling the current block's rotate function
// params: isClockWise(true, false)
void LevelTwoStrategy::rotate(bool isClockwise) {
  board.cur->rotate(isClockwise);
}