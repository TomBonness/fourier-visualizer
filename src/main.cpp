#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "Types.h"

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Fourier Series Visualizer");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    std::cout << "Fourier Visualizer - Window created!" << std::endl;

    // Create first circle
    sf::CircleShape circle1(20.f);
    circle1.setFillColor(sf::Color(255, 110, 199));  // Neon Pink
    circle1.setOrigin({20.f, 20.f});

    // Create second circle
    sf::CircleShape circle2(15.f);
    circle2.setFillColor(sf::Color(0, 240, 255));  // Cyan
    circle2.setOrigin({15.f, 15.f});

    // Animation variables
    sf::Clock clock;
    float time = 0.f;
    Point2D center(640.f, 360.f);
    float orbitRadius1 = 150.f;
    float orbitRadius2 = 80.f;

    // Trail for the path
    std::vector<Point2D> trail;
    const int maxTrailLength = 300;

    // Main loop
    while (window.isOpen()) {
        // Delta time with cap to prevent huge jumps
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.033f) deltaTime = 0.033f;  // cap at ~30 FPS worth
        time += deltaTime;

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Update first circle position (rotate around center)
        float angle1 = time;
        Point2D pos1(
            center.x + orbitRadius1 * std::cos(angle1),
            center.y + orbitRadius1 * std::sin(angle1)
        );
        circle1.setPosition(pos1.toSFML());

        // Update second circle position (rotate around first circle)
        float angle2 = time * 2.5f;  // faster rotation
        Point2D pos2(
            pos1.x + orbitRadius2 * std::cos(angle2),
            pos1.y + orbitRadius2 * std::sin(angle2)
        );
        circle2.setPosition(pos2.toSFML());

        // Add current position to trail
        trail.push_back(pos2);

        // Limit trail length
        if (trail.size() > maxTrailLength) {
            trail.erase(trail.begin());
        }

        // Clear with black background
        window.clear(sf::Color::Black);

        // Draw trail with fade effect
        if (trail.size() > 1) {
            for (size_t i = 1; i < trail.size(); i++) {
                // Linear fade - older points are more transparent
                float fade = static_cast<float>(i) / trail.size();
                int alpha = static_cast<int>(fade * 255);

                sf::Vertex line[2];
                line[0].position = trail[i - 1].toSFML();
                line[0].color = sf::Color(255, 255, 255, alpha);
                line[1].position = trail[i].toSFML();
                line[1].color = sf::Color(255, 255, 255, alpha);
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        // Draw both circles
        window.draw(circle1);
        window.draw(circle2);

        // Display
        window.display();
    }

    return 0;
}
