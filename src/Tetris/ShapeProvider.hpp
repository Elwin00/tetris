#pragma once
#include "Piece.hpp"
#include "Array2D.hpp"
#include "RandomProvider.hpp"
#include <vector>

typedef std::vector<Array2D<blockId>> Shape;

class ShapeProvider {
public:
	static Shape& getRandom();
private:
	static RandomProvider rnd;

	static Shape shapeI;
	static Shape shapeJ;
	static Shape shapeL;
	static Shape shapeT;
	static Shape shapeZ;
	static Shape shapeS;
	static Shape shapeO;
};
