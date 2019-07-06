#include "levelThreeStrategy.h"
#include <stdlib>
#include <time>

// get the next block on this level
std::unique_ptr<Block> LevelThreeStrategy::getNext() {
  srand(time(null));
  int randNum = rand() % 9;
  char blockType = 'I';
  if (randNum < 2) {
    blockType = 'S';
  } else if (randNum < 4) {
    blockType = 'Z';
  } else if (randNum < 5) {
    blockType = 'I';
  } else if (randNum < 6) {
    blockType = 'J';
  } else if (randNum < 7) {
    blockType = 'L';
  } else if (randNum < 8) {
    blockType = 'O';
  } else if (randNum < 9) {
    blockType = 'T';
  }
  return Block.create(blockType);
}

// Change the location of current block by calling the move function of block
// params: direction(n, s, e, w), distance(>=0)
void LevelOneStrategy::move(char direction, int steps) {
  board.cur->move(direction, steps);
  board.cur->move('d', 1);
}

// Perform a rotation by calling the current block's rotate function
// params: isClockWise(true, false)
void LevelOneStrategy::rotate(bool isClockwise) {
  board.cur->rotate(isClockwise);
  board.cur->move('d', 1);
}