#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Fourier Series Visualizer");
    window.setFramerateLimit(60);

    std::cout << "Fourier Visualizer - Window created!" << std::endl;

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

        // Display
        window.display();
    }

    return 0;
}
