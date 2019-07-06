#include <vector>
#include <memory>
#include "block.h"

class Board {
public:
	Board(int level);
private:
	int curLevel;
    std::unique_ptr<Block> cur;
};
