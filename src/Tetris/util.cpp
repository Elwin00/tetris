#include "stdafx.h"
#include "util.hpp"
#include "Constants.hpp"
#include "SFML/Graphics.hpp"

void configureGameText(sf::Text& text, const sf::Font* font) {
    text.setFont(*font);
    text.setFillColor(sf::Color::Cyan);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3);
    text.setCharacterSize(28);
}

void setGameTextPositionToGridCentre(sf::Text& text) {
    sf::FloatRect bounds = text.getGlobalBounds();
    float x = (CONSTANTS::GRID_COLUMNS + 2) * CONSTANTS::GFX::BLOCK_SIZE / 2 - bounds.width / 2;
    float y = (CONSTANTS::GRID_ROWS + 2) * CONSTANTS::GFX::BLOCK_SIZE / 2 - bounds.height / 2;
    text.setPosition(x, y);
}
