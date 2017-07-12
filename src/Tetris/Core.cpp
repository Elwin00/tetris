#include "stdafx.h"
#include "Core.hpp"
#include "Constants.hpp"
#include "ShapeProvider.hpp"
#include "ScoringCalculator.hpp"
#include "util.hpp"
#include <memory>

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
    state.current = Piece{ShapeProvider::getRandom(), 4, 0};
    state.next = Piece{ShapeProvider::getRandom(), 4, 0};

    unsigned int number_of_rows = CONSTANTS::GRID_ROWS;
    unsigned int number_of_columns = CONSTANTS::GRID_COLUMNS;

    // few landed blocks
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
    //state.grid.set(2, 4, 5);
    //state.grid.set(2, 5, 5);
    //state.grid.set(2, 6, 5);
    //state.grid.set(6, 4, 5);
    //state.grid.set(6, 5, 5);
    //state.grid.set(6, 6, 5);
}

void Core::render(const sf::Texture* spritesheet, const sf::Texture* shadowTexture, const std::vector<sf::Sprite>& staticSprites, const sf::Font* font) {
    mainWindow.clear(sf::Color(CONSTANTS::GFX::BACKGROUND_COLOUR));

    for (auto& sprite: staticSprites) {
        mainWindow.draw(sprite);
    }

    int size = CONSTANTS::GFX::BLOCK_SIZE;
    for (int row = 0; row < CONSTANTS::GRID_ROWS; ++row) {
        for (int col = 0; col < CONSTANTS::GRID_COLUMNS; ++col) {
            if (state.grid.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            sf::Sprite sprite(*spritesheet, sf::IntRect(state.grid.get(col, row) * size, 0, size, size));
            sprite.setPosition((1 + col) * size, (1 + row) * size);
            mainWindow.draw(sprite);
        }
    }

    auto& blocks = state.current.getCurrentRotation();
    for (int row = 0; row < blocks.height; ++row) {
        for (int col = 0; col < blocks.width; ++col) {
            blockId block = blocks.get(col, row);
            if (block == CONSTANTS::EMPTY_BLOCK) continue;
            sf::Sprite sprite(*spritesheet, sf::IntRect(block * size, 0, size, size));
            sprite.setPosition((1 + state.current.x + col) * size, (1 + state.current.y + row) * size);
            mainWindow.draw(sprite);
        }
    }

    auto nextShape = state.next.getCurrentRotation();
    auto offsetX = CONSTANTS::GRID_COLUMNS + 4 + (CONSTANTS::NEXT_COLUMNS / 2 - nextShape.width / 2);
    auto offsetY = 1 + (CONSTANTS::NEXT_ROWS / 2 - nextShape.height / 2);
    for (auto col = 0; col < nextShape.width; ++col) {
        for (auto row = 0; row < nextShape.height; ++row) {
            auto block = nextShape.get(col, row);
            if (block != CONSTANTS::EMPTY_BLOCK) {
                sf::Sprite sprite(*spritesheet, sf::IntRect(block * size, 0, size, size));
                sprite.setPosition((offsetX + col) * size, (offsetY + row) * size);
                mainWindow.draw(sprite);
            }
        }
    }

    sf::Text scoreText;
    configureGameText(scoreText, font);
    scoreText.setString("SCORE: " + std::to_string(state.score));
    sf::Text rowsText;
    configureGameText(rowsText, font);
    rowsText.setString("ROWS: " + std::to_string(state.completedRows));

    int left = (CONSTANTS::GRID_COLUMNS + 2 + 1) * CONSTANTS::GFX::BLOCK_SIZE;
    int top = (CONSTANTS::GRID_ROWS + 1) * CONSTANTS::GFX::BLOCK_SIZE - scoreText.getGlobalBounds().height - scoreText.getGlobalBounds().height;
    scoreText.setPosition(left, top);
    rowsText.setPosition(left, top + rowsText.getGlobalBounds().height);
    mainWindow.draw(scoreText);
    mainWindow.draw(rowsText);

    int originalY = state.current.y;
    hardDrop();
    if (originalY != state.current.y) {
        auto piece = state.current.getCurrentRotation();
        for (auto col = 0; col < piece.width; ++col) {
            for (auto row = 0; row < piece.height; ++row) {
                if (piece.get(col, row) != CONSTANTS::EMPTY_BLOCK) {
                    sf::Sprite sprite(*shadowTexture);
                    sprite.setPosition((1 + state.current.x + col) * size, (1 + state.current.y + row) * size);
                    mainWindow.draw(sprite);
                }
            }
        }
        state.current.y = originalY;
    }
}

void Core::gameLoop() {

    sf::Clock clock;
    const gameTime tickTime = 500; // milliseconds
    gameTime lastTickTime{0};

    font->loadFromFile(CONSTANTS::FONT_FILE);
    sf::Text endGameText;
    endGameText.setString("GAME OVER");
    sf::Text pauseGameText;
    pauseGameText.setString("PAUSED");
    configureGameText(endGameText, font.get());
    configureGameText(pauseGameText, font.get());
    setGameTextPositionToGridCentre(endGameText);
    setGameTextPositionToGridCentre(pauseGameText);

    spritesheet->loadFromFile(CONSTANTS::GFX::SPRITESHEET_BLOCKS_FILE);

    auto edgeTexture = std::make_unique<sf::Texture>();
    edgeTexture->loadFromFile(CONSTANTS::GFX::SPRITESHEET_EDGE_FILE);

    shadowTexture->loadFromFile(CONSTANTS::GFX::SPRITESHEET_SHADOW_FILE);

    std::vector<sf::Sprite> staticSprites;
    prepareGridEdges(edgeTexture.get(), staticSprites);

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
            if (phase == GamePhase::Ended) continue;

            if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)) {
                moveLeft();
            }
            if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)) {
                moveRight();
            }
            if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
                fallCurrentPiece();
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Z) {
                rotateLeft();
            }
            if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::X || event.key.code == sf::Keyboard::Up)) {
                rotateRight();
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                hardDrop();
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P && event.key.control) {
                state.isPaused = !state.isPaused;
                clock.restart();
                lastTickTime = 0;
            }
        }

        if (tick && phase != GamePhase::Ended && !state.isPaused) {
            fallCurrentPiece();
        }

        render(spritesheet.get(), shadowTexture.get(), staticSprites, font.get());

        if (phase == GamePhase::Ended) {
            mainWindow.draw(endGameText);
        }
        if (state.isPaused) {
            mainWindow.draw(pauseGameText);
        }

        mainWindow.display();
    }
}

void Core::prepareGridEdges(const sf::Texture* spritesheet, std::vector<sf::Sprite>& spriteCache) {
    int size = CONSTANTS::GFX::BLOCK_SIZE;
    sf::IntRect rect(0, 0, 24, 24);

    for (int i = 0; i < CONSTANTS::GRID_COLUMNS + 2; ++i) {
        sf::Sprite upper(*spritesheet, rect);
        upper.setPosition(size * i, 0);
        spriteCache.push_back(upper);

        sf::Sprite lower(*spritesheet, rect);
        lower.setPosition(size * i, (CONSTANTS::GRID_ROWS + 1) * size);
        spriteCache.push_back(lower);
    }

    for (int i = 1; i <= CONSTANTS::GRID_ROWS; ++i) {
        sf::Sprite left(*spritesheet, rect);
        left.setPosition(0, i * size);
        spriteCache.push_back(left);

        sf::Sprite right(*spritesheet, rect);
        right.setPosition((CONSTANTS::GRID_COLUMNS + 1) * size, i * size);
        spriteCache.push_back(right);
    }

    for (int i = 0; i <= CONSTANTS::NEXT_COLUMNS + 1; ++i) {
        sf::Sprite upper(*spritesheet, rect);
        upper.setPosition(size * (i + CONSTANTS::GRID_COLUMNS + 3), 0);
        spriteCache.push_back(upper);

        sf::Sprite lower(*spritesheet, rect);
        lower.setPosition(size * (i + CONSTANTS::GRID_COLUMNS + 3), (CONSTANTS::NEXT_ROWS + 1) * size);
        spriteCache.push_back(lower);
    }

    for (int i = 1; i <= CONSTANTS::NEXT_ROWS; ++i) {
        sf::Sprite left(*spritesheet, rect);
        left.setPosition((CONSTANTS::GRID_COLUMNS + 3) * size, i * size);
        spriteCache.push_back(left);

        sf::Sprite right(*spritesheet, rect);
        right.setPosition((CONSTANTS::GRID_COLUMNS + 4 + CONSTANTS::NEXT_COLUMNS) * size, i * size);
        spriteCache.push_back(right);
    }
}

bool Core::hasCollision() {
    auto& piece = state.current.getCurrentRotation();
    for (auto col = 0; col < piece.width; ++col) {
        for (auto row = 0; row < piece.height; ++row) {
            if (piece.get(col, row) != CONSTANTS::EMPTY_BLOCK && state.grid.get(state.current.x + col, state.current.y + row) != CONSTANTS::EMPTY_BLOCK) {
                return true;
            }
        }
    }
    return false;
}

void Core::fallCurrentPiece() {
    // gist: for every column check its lowest block. If any of the lowest blocks is on last row or there
    // is another block below, current shape lands (shape's blocks will be copied to state.grid) and next
    // shape will become current shape.
    auto& blocks = state.current.getCurrentRotation();
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

            clearLines();

            state.current = state.next;
            state.next = Piece{ShapeProvider::getRandom(), 4, 0};

            if (hasCollision()) {
                phase = GamePhase::Ended;
            }

            return;
        }
    }

    state.current.y++;
}

void Core::clearLines() {
    int row{CONSTANTS::GRID_ROWS - 1};
    int rowsCleared = 0;
    while (row >= 0) {
        bool completeRow = true;
        for (auto col = 0; col < CONSTANTS::GRID_COLUMNS - 1; ++col) {
            if (state.grid.get(col, row) == CONSTANTS::EMPTY_BLOCK) {
                completeRow = false;
                break;
            }
        }

        if (completeRow) {
            for (auto r = row - 1; r >= 0; --r) {
                for (auto col = 0; col < CONSTANTS::GRID_COLUMNS - 1; ++col) {
                    state.grid.set(col, r + 1, state.grid.get(col, r));
                    if (r == 0) {
                        state.grid.set(col, 0, CONSTANTS::EMPTY_BLOCK);
                    }
                }
            }
            ++rowsCleared;
        }
        else {
            --row;
        }
    }

    if (rowsCleared > 0) {
        state.score += ScoringCalculator::getScoreForRows(rowsCleared);
        state.completedRows += rowsCleared;
    }
}

void Core::moveLeft() {
    bool obstacleLeft = false;
    auto& blocks = state.current.getCurrentRotation();
    for (auto row = 0; row < blocks.height; ++row) {
        auto firstBlockFromLeft = 0;
        for (auto col = 0; col < blocks.width; ++col) {
            if (blocks.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            firstBlockFromLeft = col;
            break;
        }

        auto colToLeft = state.current.x + firstBlockFromLeft - 1;
        if (colToLeft >= 0 && state.grid.get(colToLeft, state.current.y + row) != CONSTANTS::EMPTY_BLOCK) {
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
    auto& blocks = state.current.getCurrentRotation();
    for (auto row = 0; row < blocks.height; ++row) {
        int firstBlockFromRight = 0;
        for (auto col = 0; col < blocks.width - 1; ++col) {
            if (blocks.get(col, row) == CONSTANTS::EMPTY_BLOCK) continue;
            firstBlockFromRight = col;
            break;
        }

        auto colToRight = state.current.x + (blocks.width - 1) - firstBlockFromRight + 1;
        if (colToRight < CONSTANTS::GRID_COLUMNS && state.grid.get(colToRight, state.current.y + row) != CONSTANTS::EMPTY_BLOCK) {
            obstacleRight = true;
            break;
        }
    }

    auto right = state.current.x + blocks.width - 1;
    if (right < CONSTANTS::GRID_COLUMNS - 1 && !obstacleRight) {
        ++state.current.x;
    }
}

void Core::rotateLeft() {
    state.current.rotateLeft();

    auto& rotation = state.current.getCurrentRotation();

    auto difX = CONSTANTS::GRID_COLUMNS - 1 - (state.current.x + static_cast<int>(rotation.width) - 1);
    if (difX < 0) {
        state.current.x += difX;
    }
}

void Core::rotateRight() {
    state.current.rotateRight();

    auto& rotation = state.current.getCurrentRotation();

    auto difX = CONSTANTS::GRID_COLUMNS - 1 - (state.current.x + static_cast<int>(rotation.width) - 1);
    if (difX < 0) {
        state.current.x += difX;
    }
}

void Core::hardDrop() {
    auto& piece = state.current.getCurrentRotation();
    while(state.current.y + piece.height - 1 < CONSTANTS::GRID_ROWS - 1) {
        ++state.current.y;
        if (hasCollision()) {
            --state.current.y;
            break;
        }
    }
}


sf::RenderWindow Core::mainWindow;
GameState Core::state;
Core::GamePhase Core::phase = GamePhase::Initializing;
std::unique_ptr<sf::Texture> Core::spritesheet = std::make_unique<sf::Texture>();
std::unique_ptr<sf::Texture> Core::shadowTexture = std::make_unique<sf::Texture>();
std::unique_ptr<sf::Font> Core::font = std::make_unique<sf::Font>();
