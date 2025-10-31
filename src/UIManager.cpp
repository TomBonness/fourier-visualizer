#include "UIManager.h"
#include <iostream>

UIManager::UIManager() : fontLoaded(false) {
}

bool UIManager::loadFont(const std::string& fontPath) {
    if (font.openFromFile(fontPath)) {
        fontLoaded = true;
        std::cout << "Font loaded successfully" << std::endl;
        return true;
    }
    std::cerr << "Failed to load font from: " << fontPath << std::endl;
    return false;
}

void UIManager::drawPanel(sf::RenderWindow& window, float x, float y, float width, float height) {
    sf::RectangleShape panel({width, height});
    panel.setPosition({x, y});
    panel.setFillColor(sf::Color(20, 20, 40, 180));  // Dark blue-ish, semi-transparent
    panel.setOutlineThickness(1.f);
    panel.setOutlineColor(sf::Color(100, 100, 150, 100));  // Subtle border
    window.draw(panel);
}

void UIManager::drawText(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size) {
    if (!fontLoaded) return;

    sf::Text textObj(font, text, size);
    textObj.setPosition({x, y});
    textObj.setFillColor(sf::Color(255, 255, 255, 200));  // White, slightly transparent
    window.draw(textObj);
}
