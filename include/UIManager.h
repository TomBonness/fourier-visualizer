#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

class UIManager {
public:
    UIManager();

    // Load font (must be called before rendering)
    bool loadFont(const std::string& fontPath);

    // Draw a semi-transparent panel
    void drawPanel(sf::RenderWindow& window, float x, float y, float width, float height);

    // Draw text at a position
    void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size = 14);

private:
    sf::Font font;
    bool fontLoaded;
};

#endif // UI_MANAGER_H
