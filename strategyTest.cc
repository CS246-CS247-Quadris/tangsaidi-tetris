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

  Level::setSeed(2);
  unique_ptr<Level> level1 = Level::create(1, board);
  level1->getNext();
  level1->getNext();
  level1->getNext();
  level1->getNext();
  level1->getNext();
  level1->move('s', 2);
  level1->rotate(true);
  level1->rotate(false);

  unique_ptr<Level> level2 = Level::create(2, board);
  level2->getNext();
  level2->getNext();
  level2->getNext();
  level2->getNext();
  level2->getNext();
  level2->move('s', 2);
  level2->rotate(true);
  level2->rotate(false);

  unique_ptr<Level> level3 = Level::create(3, board);
  level3->setNorandom(true);
  level3->setSequence("sequence.txt");
  level3->getNext();
  level3->getNext();
  level3->getNext();
  level3->getNext();
  level3->getNext();
  level3->move('s', 2);
  level3->rotate(true);
  level3->rotate(false);

  return 0;
}