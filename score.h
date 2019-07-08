#ifndef SCORE
#define SCORE

#include <iostream>
#include <utility>
#include <string>

class Score {
	int score;
	int highestScore;
	bool gameEnd;
	const std::string highestScorePath = "score.txt";

  public:
  	//requires:if data/score.txt exist, the input is a valid integer
  	Score();
    ~Score();
  	int getCurrentScore();
  	int getHighestScore();
  	//note: endGame() must be called to end score counting before deallocating all objects
  	void endGame();
  	bool gameEnded();
    void increment(int i);
};

#endif