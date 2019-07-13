#ifndef LEVELFOUR
#define LEVELFOUR

#include "level.h"
#include <memory>

class LevelFour: public LevelThree {
  private:
    int counter = 0;
  public:
    LevelFour(Board* board);

    // get the next block on this level
    std::unique_ptr<Block> getNext() override;

    ~LevelFour() override;
};

#endif