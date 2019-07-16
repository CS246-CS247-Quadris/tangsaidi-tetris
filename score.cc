#include "score.h"
#include <fstream>
using namespace std;

int Score::highestScore = 0;

Score::Score() {
	score = 0;
	gameEnd = false;
}

Score::~Score(){}

int Score::getCurrentScore() {
	return score;
}

int Score::getHighestScore() {
	return highestScore;
}

void Score::endGame() {
	gameEnd = true;
}

bool Score::gameEnded() {
	return gameEnd;
}

void Score::increment(int i) {
	score += i;
	if (score > highestScore) {
		highestScore = score;
	}
}