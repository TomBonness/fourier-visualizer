#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Types.h"

class Renderer {
public:
    Renderer();

    // Draw methods
    void drawTrail(sf::RenderWindow& window, const std::vector<Point2D>& trail);
    void drawEpicycles(sf::RenderWindow& window, const std::vector<Epicycle>& epicycles);
    void drawGlow(sf::RenderWindow& window, const Point2D& position);

private:
    // Helper function for color interpolation
    sf::Color lerpColor(sf::Color a, sf::Color b, float t);
};

#endif // RENDERER_H
