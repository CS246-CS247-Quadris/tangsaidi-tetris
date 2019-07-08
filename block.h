#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <memory>
#include <vector>
#include <utility>
#include "board.h"

class Board;

class Block {
public:
    // Take in capital chars
//    static std::unique_ptr<Block> create(char);
    void move(char, int);
    void rotate(bool);
    virtual std::vector<std::pair<int,int>> getComponents()=0;
    virtual ~Block(){}
protected:
	Block(char, Board&);
//	int orientation;
	std::pair<int, int> coordinate;
private:
	char type;
	Board& board;
	int level;
};

class IBlock : public Block {
public:
	IBlock();
};

class JBlock : public Block {
public:
	JBlock();
};

class LBlock : public Block {
public:
	LBlock();
};

class OBlock : public Block {
public:
	OBlock();
};

class SBlock : public Block {
public:
	SBlock();
};

class ZBlock : public Block {
public:
	ZBlock();
};

class TBlock : public Block {
public:
	TBlock();
};

#endif /* __BLOCK_H__ */
