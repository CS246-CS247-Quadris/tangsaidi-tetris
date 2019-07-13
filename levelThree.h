#ifndef LEVELTHREE
#define LEVELTHREE

#include "level.h"
#include <memory>

class LevelThree: public Level {
  public:
    LevelThree(Board* board);

    // get the next block on this level
    virtual std::unique_ptr<Block> getNext() override;
    // Change the location of current block by calling the move function of block
    // params: direction(n, s, e, w), distance(>=0)
    void move(char, int) override;
    // Perform a rotation by calling the current block's rotate function
    // params: isClockWise(true, false)
    void rotate(bool) override;
    ~LevelThree() override;
};

#endif