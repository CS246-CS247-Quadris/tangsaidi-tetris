#include "levelOne.h"
#include <stdlib>
#include <time>

// get the next block on this level
std::unique_ptr<Block> LevelOne::getNext() {
  srand(time(null));
  int randNum = rand() % 12;
  char blockType = 'I';
  if (randNum < 1) {
    blockType = 'S';
  } else if (randNum < 2) {
    blockType = 'Z';
  } else if (randNum < 4) {
    blockType = 'I';
  } else if (randNum < 6) {
    blockType = 'J';
  } else if (randNum < 8) {
    blockType = 'L';
  } else if (randNum < 10) {
    blockType = 'O';
  } else if (randNum < 12) {
    blockType = 'T';
  }
  return Block::create(blockType);
}

// Change the location of current block by calling the move function of block
// params: direction(n, s, e, w), distance(>=0)
void LevelOne::move(char direction, int steps) {
  board.cur->move(direction, steps);
}

// Perform a rotation by calling the current block's rotate function
// params: isClockWise(true, false)
void LevelOne::rotate(bool isClockwise) {
  board.cur->rotate(isClockwise);
}