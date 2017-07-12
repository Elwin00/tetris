#pragma once
#include <string>

namespace CONSTANTS {
    const static int SCREEN_WIDTH = 1024;
    const static int SCREEN_HEIGHT = 768;

    const static int GRID_COLUMNS = 10;
    const static int GRID_ROWS = 20;
	const static signed char EMPTY_BLOCK = -1;
    const static int NEXT_COLUMNS = 6;
    const static int NEXT_ROWS = 5;

    const static double M_PI = 3.14159265358979323846;

    namespace GFX {
        const static std::string SPRITESHEET_BLOCKS_FILE = "assets/blocks.png";
        const static std::string SPRITESHEET_EDGE_FILE = "assets/edge.png";
        const static std::string SPRITESHEET_SHADOW_FILE = "assets/shadow.png";
        const static int BLOCK_SIZE = 24; // px
        const static int BACKGROUND_COLOUR = 0x555555ff; // 85,85,85
    }

    const static std::string FONT_FILE = "assets/font/PermanentMarker.ttf";
}
