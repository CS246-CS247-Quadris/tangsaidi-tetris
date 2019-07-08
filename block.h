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
    static std::unique_ptr<Block> create(char, Board&);
    void move(char, int);
    virtual void rotate(bool);
    virtual std::vector<std::pair<int,int>> getComponents() const;
    virtual ~Block()=0;
protected:
	Block(char, Board&);
//	int orientation;
	std::vector<std::pair<int,int>> coordinate;
private:
	char type;
	Board& board;
	int level;
};

class IBlock : public Block {
public:
	IBlock(Board&);
	void rotate(bool) override;
};

class JBlock : public Block {
public:
	JBlock(Board&);
};

class LBlock : public Block {
public:
	LBlock(Board&);
};

class OBlock : public Block {
public:
	OBlock(Board&);
	void rotate(bool) override;
};

class SBlock : public Block {
public:
	SBlock(Board&);
};

class ZBlock : public Block {
public:
	ZBlock(Board&);
};

class TBlock : public Block {
public:
	TBlock(Board&);
};

#endif /* __BLOCK_H__ */
