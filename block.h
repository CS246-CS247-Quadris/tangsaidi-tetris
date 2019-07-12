#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <memory>
#include <vector>
#include <utility>
#include <memory>
#include "board.h"

class Board;
class Level;

class Block {
public:
    // Take in capital chars
    static std::unique_ptr<Block> create(char);
    void move(char, unsigned int);
    virtual void rotate(bool);
    virtual std::vector<std::pair<int,int>> getComponents() const;
    virtual ~Block()=0;
    Block& operator=(const Block&);
protected:
	Block(char);
	std::vector<std::pair<int,int>> coordinate;
private:
	char type;
//	Board& board;
	std::shared_ptr<Board> board;
	int level;
};

class IBlock : public Block {
public:
	IBlock(const std::shared_ptr<Board>&);
	void rotate(bool) override;
};

class JBlock : public Block {
public:
	JBlock(const std::shared_ptr<Board>&);
};

class LBlock : public Block {
public:
	LBlock(const std::shared_ptr<Board>&);
};

class OBlock : public Block {
public:
	OBlock(const std::shared_ptr<Board>&);
	void rotate(bool) override;
};

class SBlock : public Block {
public:
	SBlock(const std::shared_ptr<Board>&);
};

class ZBlock : public Block {
public:
	ZBlock(const std::shared_ptr<Board>&);
};

class TBlock : public Block {
public:
	TBlock(const std::shared_ptr<Board>&);
};

#endif /* __BLOCK_H__ */
