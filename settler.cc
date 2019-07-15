#include "settler.h"
#include <cmath>
using namespace std;

Settler::Settler(int curLevel, shared_ptr<Score> scr) : level{curLevel}, score{scr} {}

Settler::~Settler(){
	if (!score->gameEnded() && level >= 0) {
		int blockScore = pow((level + 1), 2);
		score->increment(blockScore);
	}
}