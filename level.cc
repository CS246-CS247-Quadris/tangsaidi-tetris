#include "level.h"
#include "levelZero.h"
#include "levelOne.h"

std::unique_ptr<Level> Level::create(int level) {
  switch(level) {
    case 0: {
      return std::make_unique<LevelZero>();
      }
    case 1: {
      return std::make_unique<levelOne>();
    }
  }		
  return std::make_unique<LevelZero>();
}