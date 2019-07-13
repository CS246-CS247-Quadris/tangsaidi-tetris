#ifndef LEVEL
#define LEVEL

#include "board.h"
#include "block.h"
#include <vector>
#include <memory>

class Block;
class Board;
class Row;

class Level {
  protected:
    Board* board;
    std::vector<char> sequence;
    int current = 0;
    bool norandom = false;
    static int seed;
    static std::string scriptFile;
    // Helper method that does not manage memory, the object it points to is managed by the unique pointer
    // on the board, which is guranteed to live longer than this created Block pointer, because it is only
    // generated when move and rotation are performed and is not stored anywhere.
    Block* getCur();
    std::vector<Row>* getBoard();
    
  public:
    Level(Board*);
    // factory method
    // param: level (0-4)
    static std::unique_ptr<Level> create(int, Board*);

    // static methods that sets configs for all
    static void setSeed(int seed);
    static void setScriptFile(std::string scriptFile);

    // setSequence and setNorandom for the norand command, vitual in case we will need to overwrite it
    virtual void setNorandom(bool norandom);
    // this is virtual so we can delete the method when necessary
    virtual void setSequence(std::string fileName);

    // get the next block on this level
    virtual std::unique_ptr<Block> getNext() = 0;
    // Change the location of current block by calling the move function of block
    // params: direction(n, s, e, w), distance(>=0)
    virtual void move(char, int);
    // Perform a rotation by calling the current block's rotate function
    // params: isClockWise(true, false)
    virtual void rotate(bool);

    virtual ~Level();
};
#endif
