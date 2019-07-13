#ifndef LEVELFOUR
#define LEVELFOUR

#include "levelThree.h"
#include <memory>

class LevelFour: public LevelThree {
  private:
    int counter = 0;
    bool hasRemovable();
  public:
    LevelFour(Board* board);

    // get the next block on this level
    std::unique_ptr<Block> getNext() override;

    ~LevelFour() override;
};

#endif