#include "stdafx.h"
#include "Core.hpp"
#include "Constants.hpp"

typedef unsigned long long gameTime;

void Core::init() {
    if (phase != GamePhase::Initializing) return;
    phase = GamePhase::Loading;

    mainWindow.create(sf::VideoMode(CONSTANTS::SCREEN_WIDTH, CONSTANTS::SCREEN_WIDTH, 32), "T3TR!$");

    initGridState();

    gameLoop();
}

// possibly obsolete function
void Core::initGridState() {
    unsigned int number_of_rows = CONSTANTS::GRID_ROWS;
    unsigned int number_of_columns = CONSTANTS::GRID_COLUMNS;

    // #GRID removed because it is initialized automatically inside GameState class
    //state.grid = std::vector<std::vector<Tile>>(CONSTANTS::GRID_COLUMNS, std::vector<Tile>(CONSTANTS::GRID_ROWS));

    // few landed blocks
    state.grid.set(3, 19, 4);
    state.grid.set(4, 19, 4);
    state.grid.set(5, 19, 4);
    state.grid.set(6, 19, 4);

    state.grid.set(7, 19, 3);
    state.grid.set(8, 19, 3);
    state.grid.set(9, 19, 3);
    state.grid.set(8, 18, 3);

    state.grid.set(0, 17, 2);
    state.grid.set(0, 18, 2);
    state.grid.set(0, 19, 2);
    state.grid.set(1, 19, 2);

    state.grid.set(2, 19, 0);
    state.grid.set(2, 18, 0);
    state.grid.set(1, 18, 0);
    state.grid.set(1, 17, 0);

    // for testing left/right collisions
    state.grid.set(2, 4, 5);
    state.grid.set(2, 5, 5);
    state.grid.set(2, 6, 5);
    state.grid.set(6, 4, 5);
    state.grid.set(6, 5, 5);
    state.grid.set(6, 6, 5);
}

void Core::render(const sf::Texture& spritesheet, std::vector<sf::Sprite>& sprites) {
    mainWindow.clear(sf::Color(CONSTANTS::GFX::BACKGROUND_COLOUR));

    //drawPitch(spritesheet);
    for (auto& sprite: sprites) {
        mainWindow.draw(sprite);
    }

    int size = CONSTANTS::GFX::SPRITE_SIZE;
    for (int row = 0; row < CONSTANTS::GRID_ROWS; ++row) {
        for (int col = 0; col < CONSTANTS::GRID_COLUMNS; ++col) {
            if (state.grid.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            sf::Sprite sprite(spritesheet, sf::IntRect(state.grid.get(col, row) * size, 0, size, size)); // TODO getting sprite for block with idx function
            sprite.setPosition((1 + col) * size, (1 + row) * size);
            mainWindow.draw(sprite);
        }
    }

    auto blocks = state.current.rotations[state.current.rotationIdx];
    for (int row = 0; row < blocks.height; ++row) {
        for (int col = 0; col < blocks.width; ++col) {
            blockId block = blocks.get(col, row);
            if (block == CONSTANTS::EMPTY_BLOCK) continue;
            sf::Sprite sprite(spritesheet, sf::IntRect(block * size, 0, size, size)); // TODO reuse created sprites, also in previous loop
            sprite.setPosition((1 + state.current.x + col) * size, (1 + state.current.y + row) * size);
            mainWindow.draw(sprite);
        }
    }
}

void Core::gameLoop() {

    sf::Clock clock;
    const gameTime tickTime = 500; // milliseconds
    gameTime lastTickTime = 0;

    std::map<std::string, sf::Sprite> sprites; // used?
    sf::Texture spritesheet;
    spritesheet.loadFromFile(CONSTANTS::GFX::SPRITESHEET_BLOCKS_FILE);

    sf::Texture edgeTexture;
    edgeTexture.loadFromFile(CONSTANTS::GFX::SPRITESHEET_EDGE_FILE);

    std::vector<sf::Sprite> staticSprites;
    drawPitch(edgeTexture, staticSprites);

    while (phase != GamePhase::Exiting) {
        gameTime elapsed = clock.getElapsedTime().asMilliseconds();
        bool tick = false;
        if (elapsed - lastTickTime > tickTime) {
            tick = true;
            lastTickTime = lastTickTime + tickTime;
        }

        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                phase = GamePhase::Exiting;
            }

            if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)) {
                moveLeft();
            }
            if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)) {
                moveRight();
            }
            if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
                fallCurrentShape();
            }
        }

        if (tick) {
            fallCurrentShape();
        }

        render(spritesheet, staticSprites);

        mainWindow.display();
    }
}

// rename to something else, this func uses 'spritesheet' to create/setup sprites, position them and then save them into 'spriteCache'
// it draws the edges around the grid
void Core::drawPitch(const sf::Texture& spritesheet, std::vector<sf::Sprite>& spriteCache) {
    int size = CONSTANTS::GFX::SPRITE_SIZE;
    sf::IntRect rect(0, 0, 24, 24);

    for (int i = 0; i < CONSTANTS::GRID_COLUMNS + 2; i++) {
        sf::Sprite upper(spritesheet, rect);
        upper.setPosition(size * i, 0);
        spriteCache.push_back(upper);

        sf::Sprite lower(spritesheet, rect);
        lower.setPosition(size * i, (CONSTANTS::GRID_ROWS + 1) * size);
        spriteCache.push_back(lower);
    }

    for (int i = 1; i <= CONSTANTS::GRID_ROWS; i++) {
        sf::Sprite left(spritesheet, rect);
        left.setPosition(0, i * size);
        spriteCache.push_back(left);
        sf::Sprite right(spritesheet, rect);
        right.setPosition((CONSTANTS::GRID_COLUMNS + 1) * size, i * size);
        spriteCache.push_back(right);
    }
}

void Core::fallCurrentShape() {
    // gist: for every column check its lowest block. If any of the lowest blocks is on last row or there
    // is another block below, current shape lands (shape's blocks will be copied to state.grid) and next
    // shape will become current shape.
    auto blocks = state.current.rotations[state.current.rotationIdx];
    for (auto col = 0; col < blocks.width; ++col) {
        int lastBlockRow;
        for (auto row = blocks.height - 1; row >= 0; --row) {
            if (blocks.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            lastBlockRow = row;
            break;
        }

        int rowBelow = state.current.y + lastBlockRow + 1;
        if (rowBelow == CONSTANTS::GRID_ROWS || state.grid.get(state.current.x + col, rowBelow) != CONSTANTS::EMPTY_BLOCK) {
            // shape cannot move downwards, it must land
            for (auto c = 0; c < blocks.width; ++c) {
                for (auto r = 0; r < blocks.height; ++r) {
                    if (blocks.get(c, r) != CONSTANTS::EMPTY_BLOCK) {
                        state.grid.set(state.current.x + c, state.current.y + r, blocks.get(c, r));
                    }
                }
            }

            state.current = state.next;
            state.next = Shape{Array2D<blockId>{{ {5,5},{5,5} }}, 4,0};
            return;
        }
    }

    state.current.y++;
}

void Core::moveLeft() {
    bool obstacleLeft = false;
    auto blocks = state.current.rotations[state.current.rotationIdx];
    for (auto row = 0; row < blocks.height; ++row) {
        auto firstBlockFromLeft = 0;
        for (auto col = 0; col < blocks.width; ++col) {
            if (blocks.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            firstBlockFromLeft = col;
            break;
        }

        auto colToLeft = state.current.x + firstBlockFromLeft - 1;
        if (state.grid.get(colToLeft, state.current.y + row) != CONSTANTS::EMPTY_BLOCK) {
            obstacleLeft = true;
            break;
        }
    }

    if (state.current.x > 0 && !obstacleLeft) {
        --state.current.x;
    }
}

void Core::moveRight() {
    bool obstacleRight = false;
    auto blocks = state.current.rotations[state.current.rotationIdx];
    for (auto row = 0; row < blocks.height; ++row) {
        int firstBlockFromRight = 0;
        for (auto col = 0; col < blocks.width - 1; ++col) {
            if (blocks.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            firstBlockFromRight = col;
            break;
        }

        auto colToRight = state.current.x + (blocks.width - 1) - firstBlockFromRight + 1;
        if (state.grid.get(colToRight, state.current.y + row) != CONSTANTS::EMPTY_BLOCK) {
            obstacleRight = true;
            break;
        }
    }

    auto right = state.current.x + blocks.width - 1;
    if (right < CONSTANTS::GRID_COLUMNS - 1 && !obstacleRight) {
        ++state.current.x;
    }
}



sf::RenderWindow Core::mainWindow;
GameState Core::state;
Core::GamePhase Core::phase = GamePhase::Initializing;
