#pragma once
#include "Piece.hpp"
#include "Array2D.hpp"
#include "Constants.hpp"
#include "ShapeProvider.hpp"


struct GameState{
	Array2D<blockId> grid{CONSTANTS::GRID_COLUMNS, CONSTANTS::GRID_ROWS, CONSTANTS::EMPTY_BLOCK};

	Piece current{ShapeProvider::getRandom(), 4, 0}; // mandatory initialization, static shape data no set at this point (why?)
	Piece next{ShapeProvider::getRandom(), 4, 0};
};
