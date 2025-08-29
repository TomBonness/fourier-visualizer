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

    // Animation variables
    sf::Clock clock;
    float time = 0.f;
    Point2D screenCenter(640.f, 360.f);

    // Create multiple epicycles (hardcoded for now)
    std::vector<Epicycle> epicycles;
    epicycles.push_back(Epicycle(100.f, 1.0f, sf::Color(255, 110, 199)));   // Pink, slow
    epicycles.push_back(Epicycle(60.f, 2.5f, sf::Color(0, 240, 255)));      // Cyan, medium
    epicycles.push_back(Epicycle(40.f, -1.5f, sf::Color(185, 103, 255)));   // Purple, reverse
    epicycles.push_back(Epicycle(25.f, 3.0f, sf::Color(255, 16, 240)));     // Hot pink, fast
    epicycles.push_back(Epicycle(15.f, -2.0f, sf::Color(5, 217, 232)));     // Electric blue

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

        // Update epicycle positions (each rotates around the previous one)
        Point2D currentPos = screenCenter;
        for (size_t i = 0; i < epicycles.size(); i++) {
            float angle = time * epicycles[i].frequency;

            // Calculate position relative to current center
            Point2D offset(
                epicycles[i].radius * std::cos(angle),
                epicycles[i].radius * std::sin(angle)
            );

            // Update epicycle center
            epicycles[i].center = currentPos;

            // Next epicycle rotates around the tip of this one
            currentPos.x += offset.x;
            currentPos.y += offset.y;
        }

        // The final position is where we draw the trail
        trail.push_back(currentPos);

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

        // Draw connecting lines between epicycles
        Point2D lineStart = screenCenter;
        for (size_t i = 0; i < epicycles.size(); i++) {
            float angle = time * epicycles[i].frequency;
            Point2D offset(
                epicycles[i].radius * std::cos(angle),
                epicycles[i].radius * std::sin(angle)
            );
            Point2D lineEnd(lineStart.x + offset.x, lineStart.y + offset.y);

            // Draw connecting arm
            sf::Vertex line[2];
            line[0].position = lineStart.toSFML();
            line[0].color = epicycles[i].color;
            line[1].position = lineEnd.toSFML();
            line[1].color = epicycles[i].color;
            window.draw(line, 2, sf::PrimitiveType::Lines);

            lineStart = lineEnd;
        }

        // Draw all epicycles
        for (const auto& epic : epicycles) {
            // Draw the circle
            sf::CircleShape circle(epic.radius);
            circle.setOrigin({epic.radius, epic.radius});
            circle.setPosition(epic.center.toSFML());
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(2.0f);
            circle.setOutlineColor(epic.color);
            window.draw(circle);

            // Draw center dot
            sf::CircleShape dot(3.f);
            dot.setOrigin({3.f, 3.f});
            dot.setPosition(epic.center.toSFML());
            dot.setFillColor(epic.color);
            window.draw(dot);
        }

        // Display
        window.display();
    }

    return 0;
}
