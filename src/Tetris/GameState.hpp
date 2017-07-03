#pragma once
#include "Shape.hpp"
#include "Array2D.hpp"
#include "Constants.hpp"

struct GameState{
	Array2D<blockId> grid{CONSTANTS::GRID_COLUMNS, CONSTANTS::GRID_ROWS, CONSTANTS::EMPTY_BLOCK};

	Shape current;
    Shape next;

	GameState() : 
		current(Shape{Array2D<blockId>{{
			{-1, 3,-1},
			{ 3, 3, 3} }}, 3,0}),
		next(Shape{Array2D<blockId>{{
			{-1, 3},
			{-1, 3},
			{ 3, 3} }}, 4,0}) {}
};
