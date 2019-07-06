#include <vector>
#include <memory>

class Board {
public:
	Board(int level);
private:
	int curLevel;
    std::unique_ptr<Block> cur;
};
