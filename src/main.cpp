#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Fourier Series Visualizer");
    window.setFramerateLimit(60);

    std::cout << "Fourier Visualizer - Window created!" << std::endl;

    // Create a simple circle
    sf::CircleShape circle(20.f);  // radius of 20
    circle.setFillColor(sf::Color::White);
    circle.setOrigin({20.f, 20.f});  // set origin to center of circle

    // Animation variables
    sf::Clock clock;
    float time = 0.f;
    float centerX = 640.f;
    float centerY = 360.f;
    float orbitRadius = 150.f;

    // Main loop
    while (window.isOpen()) {
        // Delta time
        float deltaTime = clock.restart().asSeconds();
        time += deltaTime;

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Update circle position (rotate around center)
        float angle = time;
        float x = centerX + orbitRadius * std::cos(angle);
        float y = centerY + orbitRadius * std::sin(angle);
        circle.setPosition({x, y});

        // Clear with black background
        window.clear(sf::Color::Black);

        // Draw the circle
        window.draw(circle);

        // Display
        window.display();
    }

    return 0;
}
