#pragma once
#include "SFML/Graphics.hpp"
#include "GameState.hpp"
#include <vector>


class Core final {
public:
    static void init();

private:
    static void initGridState();
    static void render(const sf::Texture* spritesheet, const sf::Texture*, const std::vector<sf::Sprite>& staticSprites, const sf::Font*);
    static void gameLoop();

    static sf::RenderWindow mainWindow;
    static GameState state;

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
