#include "stdafx.h"
#include "ScoringCalculator.hpp"


int ScoringCalculator::getScoreForRows(int rowsCompleted) {
    switch(rowsCompleted) {
        case 0: return 0;
        case 1: return 100; // 100/row
        case 2: return 250; // 150/row
        case 3: return 500; // 250/row
        case 4: return 1200; // 400/row
        default: return 0;
    }
}
