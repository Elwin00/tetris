#pragma once
#include "Array2D.hpp"

typedef signed int blockId; // ids in order as they appear on spritesheet, 0 based

class Shape {
public:
	int x{0};
	int y{0};
	int rotationIdx{0};
	std::vector<Array2D<blockId>> rotations;

	explicit Shape(Array2D<blockId>& initial) : Shape(initial, 0, 0) {}
	explicit Shape(Array2D<blockId>&& initial) : Shape(initial, 0, 0) {}
	explicit Shape(Array2D<blockId>&& initial, int x, int y) : Shape(initial, x, y) {}
	explicit Shape(Array2D<blockId>& initial, int x, int y) : Shape(std::vector<Array2D<blockId>>{1, initial}, x, y) {}
	explicit Shape(std::vector<Array2D<blockId>>& initial) : rotations(initial) {}
	explicit Shape(std::vector<Array2D<blockId>>& initial, int x, int y) : x{x}, y{y}, rotations(initial) {}
	explicit Shape(std::vector<Array2D<blockId>>&& initial, int x, int y) : x{x}, y{y}, rotations(initial) {}

	//explicit Shape(const Shape& that) {
	//	x = that.x;
	//	y = that.y;
	//	rotationIdx = that.rotationIdx;
	//	std::copy(that.rotations.begin(), that.rotations.end(), rotations.begin());
	//}

	void print() const {
		std::cout << "<drawing shape>:" << std::endl;
		rotations[rotationIdx].print();
	}
};
