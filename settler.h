#ifndef SETTLER
#define SETTLER

#include <iostream>
#include <memory>
#include "score.h"

class Settler {
	int level;
	std::shared_ptr<Score> score;

  public:
  	Settler(int, std::shared_ptr<Score>);
  	//requires the level for "*"-1-element-block is -1
  	~Settler();
};

#endif
