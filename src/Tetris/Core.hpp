#pragma once
#include "SFML/Graphics.hpp"
#include "GameState.hpp"
#include <vector>


class Core final {
public:
    static void init();

private:
    static void initGridState();
    static void render(const sf::Texture& spritesheet, std::vector<sf::Sprite>& sprites);
    static void gameLoop();

    static sf::RenderWindow mainWindow;
    static GameState state;

    static enum class GamePhase {
        Initializing,
        Loading,
        MainMenu,
        Playing,
        Exiting
    } phase;

    // helpers / temporary
    static void drawPitch(const sf::Texture& spritesheet, std::vector<sf::Sprite>& spriteCache);
	static void moveFallingTiles();
	static void fallCurrentShape();
};
