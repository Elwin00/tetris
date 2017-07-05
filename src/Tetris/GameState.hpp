#pragma once
#include "Piece.hpp"
#include "Array2D.hpp"
#include "Constants.hpp"

struct GameState{
	Array2D<blockId> grid{CONSTANTS::GRID_COLUMNS, CONSTANTS::GRID_ROWS, CONSTANTS::EMPTY_BLOCK};

	Piece current;
    Piece next;

	GameState() : 
		current(Piece{Array2D<blockId>{{
			{-1, 3,-1},
			{ 3, 3, 3} }}, 3,0}),
		next(Piece{Array2D<blockId>{{
			{-1, 3},
			{-1, 3},
			{ 3, 3} }}, 4,0}) {}
};
