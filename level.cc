#include "level.h"
#include "levelZero.h"
#include "levelOne.h"

#include <fstream>
#include <iostream>

int Level::seed = 0;
std::string Level::scriptFile = "sequence.txt";

void Level::setSequence(std::string fileName) {
  std::ifstream inFile;
  inFile.open(fileName);
  if (!inFile) {
    std::cerr << "Failed to open file" << std::endl;
    return;
  }

  char c;
  sequence = std::vector<char>();
  current = 0;
  while (inFile >> c) {
    sequence.push_back(c);
  }
  inFile.close();
}

void Level::setSeed(int seed) {
  Level::seed = seed;
}
void Level::setScriptFile(std::string scriptFile) {
  Level::scriptFile = scriptFile;
}

std::unique_ptr<Level> Level::create(int level, std::shared_ptr<Board> board) {
  switch(level) {
    case 0: {
      return std::make_unique<LevelZero>(board);
      }
    case 1: {
      return std::make_unique<LevelOne>(board);
    }
  }		
  return std::make_unique<LevelZero>(board);
}

void Level::move(char direction, int steps) {
  board->cur->move(direction, steps);
}

void Level::rotate(bool isClockwise) {
  board->cur->rotate(isClockwise);
}

Level::~Level() {}