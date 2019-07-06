#include "settler.h"
#include <cmath>
using namespace std;

Settler::Settler(int curLevel, Score & scr) : level{curLevel}, score{scr} {}

Settler::~Settler(){
	if (!score.gameEnded()) {
		int blockScore = pow((level + 1), 2);
		score.increment(blockScore);
		cout << "Destructor called, score incremented by : " << blockScore << endl; 
	}
}