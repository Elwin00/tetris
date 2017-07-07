#include "stdafx.h"
#include "Constants.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

void configureEndGameText(sf::Text& text, const std::shared_ptr<sf::Font> font) {
    text.setFont(*font);
    text.setFillColor(sf::Color::Cyan);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3);
    text.setCharacterSize(28);
    text.setString("GAME OVER");

    sf::FloatRect bounds = text.getGlobalBounds();
    float x = (CONSTANTS::GRID_COLUMNS + 2) * CONSTANTS::GFX::SPRITE_SIZE / 2 - bounds.width / 2;
    float y = (CONSTANTS::GRID_ROWS + 2) * CONSTANTS::GFX::SPRITE_SIZE / 2 - bounds.height / 2;
    text.setPosition(x, y);
}
