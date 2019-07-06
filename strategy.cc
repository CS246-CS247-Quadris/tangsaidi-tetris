#include "strategy.h"
#include "levelZeroStrategy.h"

#include <memory>

std::unique_ptr<Strategy> Strategy::create(int level) {
  switch(level) {
    case 0: {
      return std::make_unique<Strategy>(new LevelZeroStrategy())
      }
  }		
  return std::make_unique<Strategy>(new LevelZeroStrategy())
}