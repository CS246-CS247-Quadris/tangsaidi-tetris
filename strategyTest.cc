#include "level.h"

#include <memory>
#include <string>

using namespace std;

int main(void) {
  shared_ptr<Board> board = make_shared<Board>(0);
  unique_ptr<Level> level0 = Level::create(0, board);
  level0->getNext(); // J
  level0->getNext(); // I
  level0->getNext(); // T
  level0->getNext(); // J
  level0->move('s', 2);
  level0->rotate(true);
  level0->rotate(false);
  Level::setScriptFile("sequence2.txt");
  level0->setSequence("sequence2.txt");
  level0->getNext(); // O
  level0->getNext(); // O
  level0->getNext(); // O
  level0->getNext(); // O
  return 0;
}