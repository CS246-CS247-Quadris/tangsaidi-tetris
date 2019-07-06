#include "strategy.h"
#include "levelZeroStrategy.h"

std::unique_ptr<Strategy> Strategy::create(int level) {
  switch(level) {
    case 0: {
      return std::make_unique<LevelZeroStrategy>()
      }
  }		
  return std::make_unique<LevelZeroStrategy>()
}