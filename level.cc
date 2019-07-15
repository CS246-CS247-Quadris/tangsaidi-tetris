#include "level.h"
#include "levelZero.h"
#include "levelOne.h"
#include "levelTwo.h"
#include "levelThree.h"

#include <fstream>
#include <iostream>

int Level::seed = 0;
std::string Level::scriptFile = "sequence.txt";

Level::Level(Board* board): board(board) {} 


Block* Level::getCur() {
  return board->cur.get();
}

std::vector<Row>* Level::getBoard() {
  return &(board->board);
}

void Level::setNorandom(bool norandom) {
  this->norandom = norandom;
}

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

std::unique_ptr<Level> Level::create(int level, Board* board) {
  switch(level) {
    case 0: {
      return std::make_unique<LevelZero>(board);
      }
    case 1: {
      return std::make_unique<LevelOne>(board);
    }
    case 2: {
      return std::make_unique<LevelTwo>(board);
    }
    case 3: {
      return std::make_unique<LevelThree>(board);
    }
  }		
  return std::make_unique<LevelZero>(board);
}

void Level::move(char direction, int steps) {
  board->cur->move(direction, steps);
  if (!board->isValid(board->cur->getComponents())) {
    board->cur->move(direction, (-1) * steps);
  }
}

void Level::rotate(bool isClockwise) {
  board->cur->rotate(isClockwise);
  if (!board->isValid(board->cur->getComponents())) {
    board->cur->rotate(1 ^ isClockwise);
  }
}

Level::~Level() {}