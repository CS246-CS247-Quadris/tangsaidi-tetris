#include "levelFour.h"
#include <cstdlib> 
#include <utility>


#include <iostream>
using namespace std;

LevelFour::LevelFour(Board* board): LevelThree(board){
  srand(Level::seed);
}

// Check if we have removable rows
bool LevelFour::hasRemovable() {
  for (auto &r: *(getBoard())) {
    if (r.isRemovable()) {
      return true;
    }
  }
  return false;
}

// get the next block on this level, failed command also trigger heavy move
// **TEST REQUIRED**
std::unique_ptr<Block> LevelFour::getNext() {
  std::unique_ptr<Block> ret = LevelThree::getNext();
  counter++;
  if (hasRemovable()) {
    counter = 0;
  }
  if (counter != 0 && counter % 5 == 0) {
    int dropRow = 14;
    int const mid = 5;
    if (!getBoard()->at(dropRow).isOccupied(mid)) {
      for (int i = 14; i>-1; --i) {
        if (getBoard()->at(i).isOccupied(mid)) {
          dropRow++;
          break;
        } else {
          dropRow--;
        }
      }

      board->createSettler(std::make_pair(mid, dropRow));
    }
  }
  return std::move(ret);
}

LevelFour::~LevelFour() {}