#include "stdafx.h"
#include "ShapeProvider.hpp"


Shape& ShapeProvider::getRandom() {
    const unsigned shapesCount = 5;
    auto r = rnd.get(1, shapesCount);

    switch (r) {
        case 1: return shapeI;
        case 2: return rnd.get(0, 1) ? shapeJ : shapeL;
        case 3: return shapeT;
        case 4: return rnd.get(0, 1) ? shapeZ : shapeS;
        case 5: return shapeO;
        default: throw std::exception("incorrect shape index, update ShapeProvider");
    }
}

RandomProvider ShapeProvider::rnd;

Shape ShapeProvider::shapeI {
    Array2D<blockId>{{
            {6, 6, 6, 6}
        }},
    Array2D<blockId>{{
            {6}, {6}, {6}, {6}
        }}
};

Shape ShapeProvider::shapeJ {
    Array2D<blockId>{{
            {-1,  2},
            {-1,  2},
            { 2,  2}
        }},
    Array2D<blockId>{{
            {2, -1, -1},
            {2,  2,  2}
        }},
    Array2D<blockId>{{
            {2,  2},
            {2, -1},
            {2, -1}
        }},
    Array2D<blockId>{{
            { 2,  2, 2},
            {-1, -1, 2}
        }}
};

Shape ShapeProvider::shapeL {
    Array2D<blockId>{{
            {2, -1},
            {2, -1},
            {2,  2}
        }},
    Array2D<blockId>{{
            {2,  2,  2},
            {2, -1, -1}
        }},
    Array2D<blockId>{{
            {2,  2},
            {-1, 2},
            {-1, 2}
        }},
    Array2D<blockId>{{
            {-1, -1, 2},
            { 2,  2, 2}
        }}
};

Shape ShapeProvider::shapeT {
    Array2D<blockId>{{
            {-1, 3, -1},
            { 3, 3,  3}
        }},
    Array2D<blockId>{{
            {3, -1},
            {3,  3},
            {3, -1}
        }},
    Array2D<blockId>{{
            { 3, 3,  3},
            {-1, 3, -1}
        }},
    Array2D<blockId>{{
            {-1, 3},
            { 3, 3},
            {-1, 3}
        }}
};
Shape ShapeProvider::shapeZ {
    Array2D<blockId>{{
            { 4,  4, -1},
            {-1,  4,  4}
        }},
    Array2D<blockId>{{
            {-1,  4},
            { 4,  4},
            { 4, -1}
        }}
};
Shape ShapeProvider::shapeS {
    Array2D<blockId>{{
            {-1, 4,  4},
            { 4, 4, -1}
        }},
    Array2D<blockId>{{
            { 4, -1},
            { 4,  4},
            {-1,  4}
        }}
};
Shape ShapeProvider::shapeO {
    Array2D<blockId>{{
            {5, 5},
            {5, 5}
        }}
};
