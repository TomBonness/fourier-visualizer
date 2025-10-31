#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "Types.h"
#include "FourierEngine.h"
#include "PathData.h"

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

    // Create Fourier Engine
    FourierEngine fourierEngine;

    // Start with circle
    std::vector<Point2D> path = PathData::createCircle(100, 120.f);
    fourierEngine.computeDFT(path);

    std::cout << "DFT computed! Press 1-4 to switch shapes" << std::endl;

    // Trail for the path
    std::vector<Point2D> trail;
    const int maxTrailLength = 300;

    // Main loop
    while (window.isOpen()) {
        // Delta time with cap to prevent huge jumps
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.033f) deltaTime = 0.033f;  // cap at ~30 FPS worth
        time += deltaTime * 0.3f;  // Slow down animation to 30% speed

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Handle keyboard input
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                bool shapeChanged = false;

                if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    // Circle
                    path = PathData::createCircle(100, 120.f);
                    shapeChanged = true;
                    std::cout << "Shape: Circle" << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    // Square
                    path = PathData::createSquare(200, 250.f);
                    shapeChanged = true;
                    std::cout << "Shape: Square" << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    // Star
                    path = PathData::createStar(200, 5, 120.f);
                    shapeChanged = true;
                    std::cout << "Shape: Star" << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num4) {
                    // Heart
                    path = PathData::createHeart(200, 10.f);
                    shapeChanged = true;
                    std::cout << "Shape: Heart" << std::endl;
                }

                if (shapeChanged) {
                    fourierEngine.computeDFT(path);
                    trail.clear();
                    time = 0.f;
                }
            }
        }

        // Get epicycles from Fourier Engine
        std::vector<Epicycle> epicycles = fourierEngine.getEpicycles(time);

        // Assign colors to epicycles
        for (size_t i = 0; i < epicycles.size(); i++) {
            epicycles[i].color = getColorForIndex(i, epicycles.size());
        }

        // Position epicycles relative to screen center and chain them together
        Point2D currentPos = screenCenter;
        for (size_t i = 0; i < epicycles.size(); i++) {
            // Calculate rotation angle for this epicycle
            float angle = 2.0f * M_PI * epicycles[i].frequency * time;
            Point2D offset(
                epicycles[i].radius * std::cos(angle),
                epicycles[i].radius * std::sin(angle)
            );

            // Set epicycle center and move to next position
            epicycles[i].center = currentPos;
            currentPos.x += offset.x;
            currentPos.y += offset.y;
        }

        // The final position is where we draw the trail
        trail.push_back(currentPos);

        // Limit trail length
        if (trail.size() > maxTrailLength) {
            trail.erase(trail.begin());
        }

        // Clear with deep black background (vaporwave aesthetic)
        window.clear(sf::Color(10, 10, 10));  // #0a0a0a

        // Draw trail with gradient and smooth fade effect
        if (trail.size() > 1) {
            for (size_t i = 1; i < trail.size(); i++) {
                // Quadratic fade for smoother effect
                float t = static_cast<float>(i) / trail.size();
                float fade = t * t;  // Quadratic easing
                int alpha = static_cast<int>(fade * 255);

                // Color gradient along trail: Pink -> Cyan -> Purple
                sf::Color color;
                if (t < 0.5f) {
                    // Pink to Cyan
                    color = lerpColor(
                        sf::Color(255, 110, 199),  // Neon Pink
                        sf::Color(0, 240, 255),    // Cyan
                        t * 2.0f
                    );
                } else {
                    // Cyan to Purple
                    color = lerpColor(
                        sf::Color(0, 240, 255),    // Cyan
                        sf::Color(185, 103, 255),  // Purple
                        (t - 0.5f) * 2.0f
                    );
                }

                sf::Vertex line[2];
                line[0].position = trail[i - 1].toSFML();
                line[0].color = sf::Color(color.r, color.g, color.b, alpha);
                line[1].position = trail[i].toSFML();
                line[1].color = sf::Color(color.r, color.g, color.b, alpha);
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        // Draw connecting lines between epicycles
        Point2D lineStart = screenCenter;
        for (size_t i = 0; i < epicycles.size(); i++) {
            // Use same angle calculation as epicycle positioning
            float angle = 2.0f * M_PI * epicycles[i].frequency * time;
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

        // Draw glow at the drawing point (final position)
        if (!epicycles.empty()) {
            Point2D drawPoint = currentPos;

            // Multi-layer glow effect
            for (int i = 5; i >= 1; i--) {
                float radius = i * 3.0f;
                int alpha = 50 / i;  // Fade outward

                sf::CircleShape glow(radius);
                glow.setOrigin({radius, radius});
                glow.setPosition(drawPoint.toSFML());
                glow.setFillColor(sf::Color(0, 240, 255, alpha));  // Cyan glow
                window.draw(glow, sf::BlendAdd);  // Additive blending for glow
            }

            // Bright center point
            sf::CircleShape centerPoint(2.f);
            centerPoint.setOrigin({2.f, 2.f});
            centerPoint.setPosition(drawPoint.toSFML());
            centerPoint.setFillColor(sf::Color(255, 255, 255));
            window.draw(centerPoint, sf::BlendAdd);
        }

        // Display
        window.display();
    }

    return 0;
}
