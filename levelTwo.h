#ifndef LEVELTWO
#define LEVELTWO

#include "level.h"
#include <memory>

class LevelTwo: public Level {
  public:    
    LevelTwo(Board* board);
    // get the next block on this level
    std::unique_ptr<Block> getNext() override;
    ~LevelTwo() override;
};
#endif