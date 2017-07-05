#include "stdafx.h"
#include "ShapeProvider.hpp"


Shape& ShapeProvider::getRandom() {
    const unsigned shapesCount = 5;
    auto r = rnd.get(1, shapesCount);

    switch(r) {
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
            {1, 1, 1, 1}
        }},
    Array2D<blockId>{{
            {1}, {1}, {1}, {1}
        }}
};

Shape ShapeProvider::shapeJ {
    Array2D<blockId>{{
            {0, 2},
            {0, 2},
            {2, 2}
        }},
    Array2D<blockId>{{
            {2, 0, 0},
            {2, 2, 2}
        }},
    Array2D<blockId>{{
            {2, 2},
            {2, 0},
            {2, 0}
        }},
    Array2D<blockId>{{
            {2, 2, 2},
            {0, 0, 2}
        }}
};

Shape ShapeProvider::shapeL {
    Array2D<blockId>{{
            {2, 0},
            {2, 0},
            {2, 2}
        }},
    Array2D<blockId>{{
            {2, 2, 2},
            {2, 0, 0}
        }},
    Array2D<blockId>{{
            {2, 2},
            {0, 2},
            {0, 2}
        }},
    Array2D<blockId>{{
            {0, 0, 2},
            {2, 2, 2}
        }}
};

Shape ShapeProvider::shapeT {
    Array2D<blockId>{{
            {0, 3, 0},
            {3, 3, 3}
        }},
    Array2D<blockId>{{
            {3, 0},
            {3, 3},
            {3, 0}
        }},
    Array2D<blockId>{{
            {3, 3, 3},
            {0, 3, 0}
        }},
    Array2D<blockId>{{
            {0, 3},
            {3, 3},
            {0, 3}
        }}
};
Shape ShapeProvider::shapeZ {
    Array2D<blockId>{{
            {4, 4, 0},
            {0, 4, 4}
        }},
    Array2D<blockId>{{
            {0, 4},
            {4, 4},
            {4, 0}
        }}
};
Shape ShapeProvider::shapeS {
    Array2D<blockId>{{
            {0, 4, 4},
            {4, 4, 0}
        }},
    Array2D<blockId>{{
            {4, 0},
            {4, 4},
            {0, 4}
        }}
};
Shape ShapeProvider::shapeO {
    Array2D<blockId>{{
            {5, 5},
            {5, 5}
        }}
};
