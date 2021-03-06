#pragma once
#include "SFML/Graphics.hpp"
#include "GameState.hpp"
#include <vector>
#include <memory>


class Core final {
public:
    static void init();

private:
    static void initGridState();
    static void render(const std::vector<sf::Sprite>& staticSprites);
    static void gameLoop();

    static sf::RenderWindow mainWindow;
    static GameState state;
    static std::unique_ptr<sf::Texture> spritesheet;
    static std::unique_ptr<sf::Texture> shadowTexture;
    static std::unique_ptr<sf::Font> font;

    static enum class GamePhase {
        Initializing,
        Loading,
        MainMenu,
        Playing,
        Ended,
        Exiting
    } phase;

    // helpers / temporary
    static void prepareGridEdges(const sf::Texture* spritesheet, std::vector<sf::Sprite>& spriteCache);
    static bool hasCollision();
    static void fallCurrentPiece();
    static void clearLines();
    static void moveLeft();
    static void moveRight();
    static void rotateLeft();
    static void rotateRight();
    static void hardDrop();
};
