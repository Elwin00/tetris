#include "stdafx.h"
#include "Piece.hpp"

const Array2D<blockId>& Piece::getCurrentRotation() const {
    return rotations[rotationIdx];
}

void Piece::rotateLeft() {
    --rotationIdx;
    if (rotationIdx < 0) {
        rotationIdx = rotations.size() - 1;
    }
}

void Piece::rotateRight() {
    ++rotationIdx;
    if (rotationIdx == rotations.size()) {
        rotationIdx = 0;
    }
}
