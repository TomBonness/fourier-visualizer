#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "Types.h"

// Simple color interpolation function
sf::Color lerpColor(sf::Color a, sf::Color b, float t) {
    return sf::Color(
        static_cast<uint8_t>(a.r + (b.r - a.r) * t),
        static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t)
    );
}

// Get color for epicycle based on index
sf::Color getColorForIndex(int index, int total) {
    float t = static_cast<float>(index) / (total - 1);

    // Vaporwave color gradient: Pink -> Cyan -> Purple
    if (t < 0.5f) {
        // Pink to Cyan
        return lerpColor(
            sf::Color(255, 110, 199),  // Neon Pink
            sf::Color(0, 240, 255),    // Cyan
            t * 2.0f
        );
    } else {
        // Cyan to Purple
        return lerpColor(
            sf::Color(0, 240, 255),    // Cyan
            sf::Color(185, 103, 255),  // Purple
            (t - 0.5f) * 2.0f
        );
    }
}

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
    int numEpicycles = 7;

    // Define epicycles with auto-generated gradient colors
    // Tuned for nice visual balance
    float radii[] = {120.f, 70.f, 50.f, 35.f, 22.f, 15.f, 10.f};
    float frequencies[] = {1.0f, -2.0f, 3.0f, -1.5f, 4.0f, 2.5f, -3.5f};

    for (int i = 0; i < numEpicycles; i++) {
        sf::Color color = getColorForIndex(i, numEpicycles);
        epicycles.push_back(Epicycle(radii[i], frequencies[i], color));
    }

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
