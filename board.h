#include <vector>
#include <memory>
#include "block.h"

class Board {
	public:
		std::unique_ptr<Block> cur;
		Board(int level);


	private:
		int curLevel;
};
