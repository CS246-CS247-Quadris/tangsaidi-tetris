#ifndef LEVELONE
#define LEVELONE

#include "level.h"
#include <memory>

class LevelOne: public Level {
  public:
    LevelOne(Board* board);

    // get the next block on this level
    std::unique_ptr<Block> getNext() override;
    ~LevelOne() override;
};
#endif