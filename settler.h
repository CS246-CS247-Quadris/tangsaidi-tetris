#ifndef SETTLER
#define SETTLER

#include <iostream>
#include "score.h"

class Settler {
	int level;
	Score & score;

  public:
  	Settler(int, Score &);
  	~Settler();
};

#endif