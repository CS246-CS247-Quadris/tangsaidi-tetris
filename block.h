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
    static std::unique_ptr<Block> create(char);
    char getBlockType() const;
    int getBlockLevel() const;
    void move(char, int);
    virtual void rotate(bool);
    virtual std::vector<std::pair<int,int>> getComponents() const;
    virtual ~Block()=0;
    Block& operator=(const Block&);
protected:
	Block(char);
	std::vector<std::pair<int,int>> coordinate;
private:
	char type;
	int level;
};

class IBlock : public Block {
public:
	IBlock();
	void rotate(bool) override;
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
	void rotate(bool) override;
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
