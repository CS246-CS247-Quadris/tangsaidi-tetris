#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <memory>
#include <vector>
#include <utility>
#include <memory>
#include "board.h"

class Board;
class Level;

// TODO: Add level control interface
class Block {
public:
    // Take in capital chars
    static std::unique_ptr<Block> create(char, int);
    void move(char, int);
    virtual void rotate(bool);
    virtual std::vector<std::pair<int,int>> getComponents() const;
    virtual ~Block()=0;
	char getBlockType();
	int getBlockLevel();
protected:
	Block(char, int);
	std::vector<std::pair<int,int>> coordinate;
	std::pair<int,int> rotCenter;
private:
	char type;
	int level;
};

class IBlock : public Block {
public:
	IBlock(int);
	void rotate(bool) override;
};

class JBlock : public Block {
public:
	JBlock(int);
};

class LBlock : public Block {
public:
	LBlock(int);
};

class OBlock : public Block {
public:
	OBlock(int);
	void rotate(bool) override;
};

class SBlock : public Block {
public:
	SBlock(int);
};

class ZBlock : public Block {
public:
	ZBlock(int);
};

class TBlock : public Block {
public:
	TBlock(int);
};

#endif /* __BLOCK_H__ */
