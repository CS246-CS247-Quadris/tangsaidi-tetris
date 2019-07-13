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
  	void setRowAt(int, char, std::shared_ptr<Settler>);
  	bool isRemovable();
    bool isOccupied(int);

  	friend std::ostream& operator<<(std::ostream &, const Row &);
};

#endif