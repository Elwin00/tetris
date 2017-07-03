#include "stdafx.h"
#include "Core.hpp"
#include "Shape.hpp"

int main()
{

    /*
    // TODO move to extra 'array tests' file
    Array2D<long> arrStaticSize{4, 5, 11};
    arrStaticSize.print();

    std::vector<std::vector<int>> v{
        {0,0,0},
        {0,1}, // missing elements implictly added
        {1,1,1},
        {0,0,0}
    };

    Array2D<int> arr2D{v};
    std::cout << "passed vector:" << std::endl;
    arr2D.print();

    Array2D<int> arrCpyCtor{arr2D};
    arrCpyCtor.set(0, 0, 3);
    arrCpyCtor.set(1, 0, 3);
    std::cout << "copy constructor + changes:" << std::endl;
    arrCpyCtor.print();

    std::cout << "with inline vector:" << std::endl;
    Array2D<int> arrInlineVector{std::vector<std::vector<int>>{
        {1,2,3},
        {3,4,5}
    }};
    arrInlineVector.print();

    std::cout << "implicit conversion (converting constructor):" << std::endl;
    Array2D<int> arrImplicit{{
        {11, 22,  33},
        {44, 55, -66}
    }};
    arrImplicit.print();

    Shape s{arr2D};
    s.print();
    */

    Core::init();
    return 0;
}
