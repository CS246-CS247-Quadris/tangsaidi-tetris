#include "levelFour.h"
#include <cstdlib> 


LevelFour::LevelFour(std::shared_ptr<Board> board): LevelThree(board){
  srand(Level::seed);
}

// get the next block on this level
std::unique_ptr<Block> LevelOne::getNext() {
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

LevelOne::~LevelOne() {}