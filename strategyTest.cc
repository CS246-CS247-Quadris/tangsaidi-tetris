#include "level.h"

#include <memory>

using namespace std;

int main(void) {
    shared_ptr<Board> board = make_shared<Board>(0);
    unique_ptr<Level> level0 = Level::create(0, board);
    return 0;
}