#ifndef BLOCK
#define BLOCK

#include <memory>

class Block {
  public:
    // Take in capital chars
    static std::unique_ptr<Block> create(char);
    void move(char, int);
    void rotate(bool);
};

#endif