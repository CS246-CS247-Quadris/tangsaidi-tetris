#include "levelTwo.h"
#include <cstdlib> 

LevelTwo::LevelTwo(Board* board): Level(board) {
  srand(Level::seed);
}

// get the next block on this level
std::unique_ptr<Block> LevelTwo::getNext() {
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
  return Block::create(blockType);
}

LevelTwo::~LevelTwo() {}