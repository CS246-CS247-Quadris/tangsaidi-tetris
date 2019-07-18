#ifndef ROW
#define ROW

#include <iostream>
#include <vector>
#include "pixel.h"

#define COLUMNNO 11

class Row {
	std::vector<Pixel> data;

  public:
  	Row();
  	Row(const Row &);
	  Row(Row &&);
	  Row& operator=(const Row &);
	  Row& operator=(Row &&);
  	~Row();
    /*  Row::setRowAt
        set pixel at index with passed in data.
     */
  	void setRowAt(int, char, std::shared_ptr<Settler>);
    /*  Row::clearPixelAt
        set pixel at index to be unoccupied.
     */
    void clearPixelAt(int);
    /*  Row::isRemovable
        return true if a line is completely filled and can be cleared.
     */
  	bool isRemovable();
    /*  Row::setRowAt
        return true if pixel at index is occupied, false otherwise.
     */
    bool isOccupied(int);
    /*  Row::getData
        return blockType of pixel at index i.
     */
	  char getData(int);
};

#endif