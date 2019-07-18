#ifndef PIXEL
#define PIXEL

#include <iostream>
#include "settler.h"

class Pixel {
	std::shared_ptr<Settler> parent;
	char data;

  public:
  	Pixel();
  	/* 	Pixel::setPixel
  		set pixel to passed blockType and corresponding Settler
  	 */
  	void setPixel(char, std::shared_ptr<Settler>);
  	/* 	Pixel::clearPixel
  		clear pixel to be unoccupied, i.e. default state
  	 */
  	void clearPixel();
  	~Pixel();
  	/* 	Pixel::getData
  		return the block type of pixel
  	 */
	char getData() const;
	/* 	Pixel::isOccupied
  		return true if pixel is being occupied by a settler, false otherwise
  	 */
  	bool isOccupied() const;
};

#endif