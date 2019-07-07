#ifndef LEVELTWO
#define LEVELTWO

#include "level.h"
#include <memory>
#include <vector>


class LevelTwo: public Level {
  public:    
    LevelTwo(std::shared_ptr<Board> board);
    // get the next block on this level
    std::unique_ptr<Block> getNext() override;
    ~LevelTwo() override;
};
#endif