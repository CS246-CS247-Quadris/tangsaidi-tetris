#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <memory>
#include "block.h"

class Board {
	friend class Level;

	public:
		Board(int level);


	private:
		std::unique_ptr<Block> cur;
		int curLevel;
};

#endif /* __BOARD_H__ */
