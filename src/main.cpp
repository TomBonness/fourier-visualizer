#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Fourier Series Visualizer");
    window.setFramerateLimit(60);

    std::cout << "Fourier Visualizer - Window created!" << std::endl;

    // Create first circle
    sf::CircleShape circle1(20.f);
    circle1.setFillColor(sf::Color::White);
    circle1.setOrigin({20.f, 20.f});

    // Create second circle
    sf::CircleShape circle2(15.f);
    circle2.setFillColor(sf::Color::White);
    circle2.setOrigin({15.f, 15.f});

    // Animation variables
    sf::Clock clock;
    float time = 0.f;
    float centerX = 640.f;
    float centerY = 360.f;
    float orbitRadius1 = 150.f;
    float orbitRadius2 = 80.f;

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

        // Update first circle position (rotate around center)
        float angle1 = time;
        float x1 = centerX + orbitRadius1 * std::cos(angle1);
        float y1 = centerY + orbitRadius1 * std::sin(angle1);
        circle1.setPosition({x1, y1});

        // Update second circle position (rotate around first circle)
        float angle2 = time * 2.5f;  // faster rotation
        float x2 = x1 + orbitRadius2 * std::cos(angle2);
        float y2 = y1 + orbitRadius2 * std::sin(angle2);
        circle2.setPosition({x2, y2});

        // Clear with black background
        window.clear(sf::Color::Black);

        // Draw both circles
        window.draw(circle1);
        window.draw(circle2);

        // Display
        window.display();
    }

    return 0;
}
