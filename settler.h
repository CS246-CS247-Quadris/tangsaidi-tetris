#ifndef SETTLER
#define SETTLER

#include <iostream>
#include "score.h"

class Settler {
	int level;
	Score & score;

  public:
  	Settler(int, Score &);
  	//requires the level for "*"-1-element-block is -1
  	~Settler();
};

#endif