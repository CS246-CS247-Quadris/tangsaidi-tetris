#include "pixel.h"
using namespace std;

//note here use '-' as an invalid character for pixel
Pixel::Pixel() : data{'-'}, parent{nullptr} {}

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

ostream& operator<<(std::ostream & out, const Pixel & p) {
	if (p.isOccupied()) {
		out << p.data;
	} else {
		out << ' ';
	}
	return out;
}