#ifndef PIXEL
#define PIXEL

#include <iostream>
#include "settler.h"

class Pixel {
	std::shared_ptr<Settler> parent;
	char data;

  public:
  	Pixel();
  	void setPixel(char, std::shared_ptr<Settler>);
  	void clearPixel();
  	~Pixel();
	  char getData() const;
  	bool isOccupied() const;
};

#endif