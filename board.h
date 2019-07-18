#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <memory>
#include <fstream>
#include <set>
#include <sstream>
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
		~Board();
		int getCurrentLevel() const;
		bool checkEnd();
		bool isValid(std::vector<std::pair<int, int>> coord);
		void hint();
		void drop(); // remember to clear rows only after getNext()
		void move(char direction, int steps);
		void rotate(bool isClockWise);
		void changeLevel(int delta);
		void print();
		void norand(bool isNoRandom, std::string file="");
		void setSeed(int seed);
		void replaceCurrentBlock(char cType);

		void createSettler(std::pair<int, int> coord);
		void createSettler(std::vector<std::pair<int, int>> coord, char blockType, int blockLevel);
		void deleteHintSettler();
		int getHiScore() const;
		int getScore() const;
		std::string getStringified() const;
		std::string getStringifiedNext() const;
		std::stringstream toStringStream();

	private:
		std::shared_ptr<Score> score;
		std::vector<Row> board;
		std::vector<std::vector<bool>> hintBoard;
		// std::vector<std::pair<int, int>> hintBlock;
		int curLevel;		
		std::unique_ptr<Block> cur;
		std::unique_ptr<Block> next;
		std::unique_ptr<Level> strategy;
		std::vector<std::pair<int, int>> hintSettlerCoord;
		std::string stringified;
		
		std::stringstream printRow(int y);
		std::vector<std::pair<int, int>> ifDropNow(const std::vector<std::pair<int,int>> &);
		bool isHole(int, int, int);
		int countHalfHoles(int, int, int);
		bool hintIsRemovable(int);
		int checkAndRemoveRow();
		std::pair<int, int> findHoles();
		int findEdges();
		std::pair<int, int> findHeight();
		void setHintBoard(const std::vector<std::pair<int, int>> &);
		int getHintScore();
		std::vector<std::pair<int,int>> singleOrientationHint(bool);
		void createHintSettler(std::vector<std::pair<int, int>> coord);
};

#endif /* __BOARD_H__ */
