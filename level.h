#ifndef LEVEL
#define LEVEL

#include <memory>

class Board;
class Block;
class LevelZero;

class Level {
  protected:
    std::shared_ptr<Board> board;

  public:
    // factory method
    // param: level (0-4)
    static std::unique_ptr<Level> create(int, std::shared_ptr<Board>);

    // get the next block on this level
    virtual std::unique_ptr<Block> getNext() = 0;
    // Change the location of current block by calling the move function of block
    // params: direction(n, s, e, w), distance(>=0)
    virtual void move(char, int) = 0;
    // Perform a rotation by calling the current block's rotate function
    // params: isClockWise(true, false)
    virtual void rotate(bool) = 0;
};
#endif