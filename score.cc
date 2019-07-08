#include "score.h"
#include <fstream>
using namespace std;

Score::Score() {
	score = 0;
	gameEnd = false;
	ifstream scoreFile(highestScorePath);
	if (scoreFile.is_open()) {
		scoreFile >> highestScore;
		scoreFile.close();
	} else {
		ofstream outfile(highestScorePath);
		outfile << 0 << std::endl;
		outfile.close();
		highestScore = 0;
	}
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
		ofstream scoreFile;
		scoreFile.open(highestScorePath, ios::out | ios::trunc);
		scoreFile << highestScore;
		scoreFile.close();
	}
}