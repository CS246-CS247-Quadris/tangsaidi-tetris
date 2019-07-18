#include "pixel.h"
using namespace std;

//Note: use '-' as an invalid character for pixel, unoccupied
Pixel::Pixel() : parent{nullptr}, data{'-'} {}

void Pixel::setPixel(char d, shared_ptr<Settler> s){
	data = d;
	parent = s;
}

void Pixel::clearPixel() {
	data = '-';
	parent = nullptr;
}

Pixel::~Pixel(){}

bool Pixel::isOccupied() const {
	if (parent == nullptr) return false;
	else return true;
}

char Pixel::getData() const {
	return data;
}