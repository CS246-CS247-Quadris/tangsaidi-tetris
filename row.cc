#include "row.h"
using namespace std;

Row::Row() {
	for (int i = 0; i < COLUMNNO; ++i) {
		Pixel temp;
		data.emplace_back(temp);
	}
}

Row::Row(const Row &other) : data{other.data} {}

Row::Row(Row &&other) : data{std::move(other.data)} {}

Row& Row::operator=(const Row &other) {
    this->data = other.data;
    return *this;
}

Row& Row::operator=(Row &&other) {
    this->data = other.data;
    return *this;
}

Row::~Row() {}

void Row::setRowAt(int index, char d, shared_ptr<Settler> s) {
	data.at(index).setPixel(d, s);
}

void Row::clearPixelAt(int index) {
	data.at(index).clearPixel();
}

char Row::getData(int i) {
	if (i >= (int) data.size() || i < 0) return -1;
	return data.at(i).getData();
}

bool Row::isRemovable() {
	for (auto &d : data) {
		if (!d.isOccupied()) return false;
	}
	return true;
}

bool Row::isOccupied(int i) {
	if (i >= (int) data.size() || i < 0) return false;
	return data.at(i).isOccupied();
}
