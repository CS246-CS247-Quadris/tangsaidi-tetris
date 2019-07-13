#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <memory>
#include <fstream>
#include "block.h"
#include "score.h"
#include "row.h"
#include "level.h"
#include "settler.h"


class Block;

class Board {
	friend class Level;

	public:
		Board(int level, const std::string& script);
		int getCurrentLevel() const;
		bool checkWin();
		bool isValid(std::vector<std::pair<int, int>> coord);
		void hint();
		void drop(); // remember to clear rows only after getNext()
		void move(char direction, int steps);
		void rotate(bool isClockWise);
		void changeLevel(int delta);
		void print();

		void createSettler(std::pair<int, int> coord);
		void createSettler(std::vector<std::pair<int, int>> coord, char blockType, int blockLevel);

	private:
		std::shared_ptr<Score> score;
		std::vector<Row> board;
		int curLevel;		
		std::unique_ptr<Block> cur;
		std::unique_ptr<Block> next;
		std::unique_ptr<Level> strategy;

		static const char level1DistrTable[12];
};

#endif /* __BOARD_H__ */
