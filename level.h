#ifndef LEVEL
#define LEVEL

#include "board.h"
#include "block.h"
#include <memory>

class Block;

class Level {
  protected:
    std::shared_ptr<Board> board;
    std::vector<char> sequence;
    int current = 0;
    static int seed;
    static std::string scriptFile;
  public:
    // factory method
    // param: level (0-4)
    static std::unique_ptr<Level> create(int, std::shared_ptr<Board>);

    static void setSeed(int seed);
    static void setScriptFile(std::string scriptFile);

    // this is virtual so we can delete the method when necessary
    virtual void setSequence(std::string fileName);
    // get the next block on this level
    virtual std::unique_ptr<Block> getNext() = 0;
    // Change the location of current block by calling the move function of block
    // params: direction(n, s, e, w), distance(>=0)
    virtual void move(char, int) = 0;
    // Perform a rotation by calling the current block's rotate function
    // params: isClockWise(true, false)
    virtual void rotate(bool) = 0;

    virtual ~Level();
};
#endif