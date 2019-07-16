#ifndef SCORE
#define SCORE

#include <iostream>
#include <utility>
#include <string>

class Score {
	int score;
	static int highestScore;
	bool gameEnd;

  public:
  	//requires:if data/score.txt exist, the input is a valid integer
  	Score();
    ~Score();
  	int getCurrentScore();
  	static int getHighestScore();
  	//note: endGame() must be called to end score counting before deallocating all objects
  	void endGame();
  	bool gameEnded();
    void increment(int i);
};

#endif