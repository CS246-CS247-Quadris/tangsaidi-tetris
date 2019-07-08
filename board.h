#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <memory>
#include "block.h"
#include "score.h"
#include "level.h"
#include "row.h"

class Block;

class Board {
	friend class Level;

	public:
		Board(int level);
		int getCurrentLevel() const;
		bool checkWin();
		bool isValid(std::vector<std::pair<int, int>> coord);
		void hint();
		void drop();
		void move(char direction, int steps);
		void rotate(bool isClockWise);
		void changeLevel(int delta);

	private:
		std::shared_ptr<Score> score;
		std::vector<Row> board;
		int curLevel;		
		std::unique_ptr<Block> cur;
		std::unique_ptr<Block> next;
		std::unique_ptr<Level> strategy;

		void createSettler(std::vector<std::pair<int, int>> coord, char blockType, int blockLevel);
};

#endif /* __BOARD_H__ */
