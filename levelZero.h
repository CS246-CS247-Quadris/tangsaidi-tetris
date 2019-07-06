#ifndef LEVELZERO
#define LEVELZERO

#include "level.h"
#include <memory>
#include <vector>

class Board;
class Block;

class LevelZero: public Level {
  private:
    std::vector<char> sequence;
    int current = 0;

  public:
    LevelZero(std::shared_ptr<Board> board);
    
    // set the sequence of the file
    void setSequence(std::string fileName);

    // get the next block on this level
    std::unique_ptr<Block> getNext() override;
    // Change the location of current block by calling the move function of block
    // params: direction(n, s, e, w), distance(>=0)
    void move(char, int) override;
    // Perform a rotation by calling the current block's rotate function
    // params: isClockWise(true, false)
    void rotate(bool) override;
};
#endif
