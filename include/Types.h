#ifndef TYPES_H
#define TYPES_H

#include <SFML/Graphics.hpp>

// Simple 2D point structure
struct Point2D {
    float x, y;

    Point2D(float x = 0.f, float y = 0.f) : x(x), y(y) {}

    // Convert to SFML Vector2f
    sf::Vector2f toSFML() const {
        return sf::Vector2f(x, y);
    }

    // Create from SFML Vector2f
    static Point2D fromSFML(sf::Vector2f v) {
        return Point2D(v.x, v.y);
    }
};

// Epicycle data structure
struct Epicycle {
    Point2D center;      // Current center position
    float radius;        // Circle radius
    float frequency;     // Rotation frequency
    float phase;         // Current phase/angle
    sf::Color color;     // Display color

    Epicycle(float radius = 0.f, float frequency = 0.f, sf::Color color = sf::Color::White)
        : center(0.f, 0.f), radius(radius), frequency(frequency), phase(0.f), color(color) {}
};

#endif // TYPES_H
