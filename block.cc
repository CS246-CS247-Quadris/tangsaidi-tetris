#include "block.h"

std::unique_ptr<Block> Block::create(char type) {
  return std::make_unique<Block>();
}
