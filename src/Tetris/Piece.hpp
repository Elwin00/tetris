#pragma once
#include "Array2D.hpp"

typedef signed int blockId; // ids in order as they appear on spritesheet, 0 based

class Piece {
public:
	int x{0};
	int y{0};
	int rotationIdx{0};
	std::vector<Array2D<blockId>> rotations;

	explicit Piece(Array2D<blockId>& initial) : Piece(initial, 0, 0) {}
	explicit Piece(Array2D<blockId>&& initial) : Piece(initial, 0, 0) {}
	explicit Piece(Array2D<blockId>&& initial, int x, int y) : Piece(initial, x, y) {}
	explicit Piece(Array2D<blockId>& initial, int x, int y) : Piece(std::vector<Array2D<blockId>>{1, initial}, x, y) {}
	explicit Piece(std::vector<Array2D<blockId>>& initial) : rotations(initial) {}
	explicit Piece(std::vector<Array2D<blockId>>& initial, int x, int y) : x{x}, y{y}, rotations(initial) {}
	explicit Piece(std::vector<Array2D<blockId>>&& initial, int x, int y) : x{x}, y{y}, rotations(initial) {}

	const Array2D<blockId>& getCurrentRotation() const;
	void rotateLeft();
	void rotateRight();
};
