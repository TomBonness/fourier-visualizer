#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Fourier Series Visualizer");
    window.setFramerateLimit(60);

    std::cout << "Fourier Visualizer - Window created!" << std::endl;

    // Create a simple circle in the center
    sf::CircleShape circle(50.f);  // radius of 50
    circle.setFillColor(sf::Color::White);
    circle.setPosition({640.f - 50.f, 360.f - 50.f});  // center at (640, 360)

    // Main loop
    while (window.isOpen()) {
        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Clear with black background
        window.clear(sf::Color::Black);

        // Draw the circle
        window.draw(circle);

        // Display
        window.display();
    }

    return 0;
}
