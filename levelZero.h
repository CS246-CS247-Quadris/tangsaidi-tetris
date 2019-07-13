#ifndef LEVELZERO
#define LEVELZERO

#include "level.h"
#include <memory>

class LevelZero: public Level {

  public:
    LevelZero(Board* board);
    
    void setSequence(std::string fileName) override;
    // get the next block on this level
    std::unique_ptr<Block> getNext() override;
    ~LevelZero() override;
};
#endif
