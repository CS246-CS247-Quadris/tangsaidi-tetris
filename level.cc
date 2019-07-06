#include "level.h"
#include "levelZero.h"


std::unique_ptr<Level> Level::create(int level, std::shared_ptr<Board> board) {
  switch(level) {
    case 0: {
      return std::make_unique<LevelZero>(board);
      }
  }		
  return std::make_unique<LevelZero>(board);
}