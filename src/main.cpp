#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "Types.h"
#include "FourierEngine.h"
#include "PathData.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "UIManager.h"

// Helper function for color interpolation
sf::Color lerpColor(sf::Color a, sf::Color b, float t) {
    return sf::Color(
        static_cast<uint8_t>(a.r + (b.r - a.r) * t),
        static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t)
    );
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

    // Create Fourier Engine, Renderer, Input Handler, and UI Manager
    FourierEngine fourierEngine;
    Renderer renderer;
    InputHandler inputHandler;
    UIManager uiManager;

    // Try to load a system font (macOS)
    if (!uiManager.loadFont("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cout << "Warning: UI text will not be displayed" << std::endl;
    }

    // Start with circle
    std::vector<Point2D> path = PathData::createCircle(100, 120.f);
    fourierEngine.computeDFT(path);

    std::cout << "DFT computed! Press 1-4 to switch shapes" << std::endl;

    // Trail for the path
    std::vector<Point2D> trail;
    const int maxTrailLength = 300;

    // Track drawing state
    bool wasDrawing = false;

    // Animation state
    bool paused = false;
    float speed = 0.3f;  // Animation speed multiplier
    int numEpicyclesToShow = 100;  // Number of epicycles to display

    // Visibility toggles
    bool showEpicycles = true;
    bool showTrail = true;

    // Main loop
    while (window.isOpen()) {
        // Delta time with cap to prevent huge jumps
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.033f) deltaTime = 0.033f;  // cap at ~30 FPS worth
        if (!paused) {
            time += deltaTime * speed;
        }

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Handle input (mouse drawing)
            inputHandler.handleEvent(*event, window);

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
                else if (keyPressed->code == sf::Keyboard::Key::C) {
                    // Clear drawing and reset to circle
                    inputHandler.clearPath();
                    path = PathData::createCircle(100, 120.f);
                    fourierEngine.computeDFT(path);
                    trail.clear();
                    time = 0.f;
                    std::cout << "Cleared - back to circle" << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::R) {
                    // Reset animation
                    trail.clear();
                    time = 0.f;
                    std::cout << "Reset animation" << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Space) {
                    // Toggle pause
                    paused = !paused;
                    std::cout << (paused ? "Paused" : "Playing") << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Equal || keyPressed->code == sf::Keyboard::Key::Hyphen) {
                    // Adjust speed with +/- keys
                    if (keyPressed->code == sf::Keyboard::Key::Equal) {
                        speed += 0.1f;
                        if (speed > 2.0f) speed = 2.0f;  // Max speed
                    } else {
                        speed -= 0.1f;
                        if (speed < 0.1f) speed = 0.1f;  // Min speed
                    }
                    std::cout << "Speed: " << speed << "x" << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::RBracket || keyPressed->code == sf::Keyboard::Key::LBracket) {
                    // Adjust epicycle count with [ and ] keys
                    if (keyPressed->code == sf::Keyboard::Key::RBracket) {
                        numEpicyclesToShow += 10;
                        if (numEpicyclesToShow > 200) numEpicyclesToShow = 200;  // Max epicycles
                    } else {
                        numEpicyclesToShow -= 10;
                        if (numEpicyclesToShow < 1) numEpicyclesToShow = 1;  // Min epicycles
                    }
                    std::cout << "Epicycles: " << numEpicyclesToShow << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::E) {
                    // Toggle epicycles visibility
                    showEpicycles = !showEpicycles;
                    std::cout << "Epicycles: " << (showEpicycles ? "Visible" : "Hidden") << std::endl;
                }
                else if (keyPressed->code == sf::Keyboard::Key::T) {
                    // Toggle trail visibility
                    showTrail = !showTrail;
                    std::cout << "Trail: " << (showTrail ? "Visible" : "Hidden") << std::endl;
                }

                if (shapeChanged) {
                    fourierEngine.computeDFT(path);
                    trail.clear();
                    time = 0.f;
                }
            }
        }

        // Check if user just finished drawing
        if (wasDrawing && !inputHandler.isDrawing()) {
            const auto& drawnPath = inputHandler.getDrawnPath();
            if (drawnPath.size() > 10) {  // Only if they drew enough points
                // Resample and center the path around origin
                path = PathData::resamplePath(drawnPath, 200);
                path = PathData::centerPath(path, Point2D(0.f, 0.f));

                // Compute Fourier transform
                fourierEngine.computeDFT(path);
                trail.clear();
                time = 0.f;

                std::cout << "Computed DFT from drawn shape!" << std::endl;
            }
        }
        wasDrawing = inputHandler.isDrawing();

        // Get epicycles from Fourier Engine
        std::vector<Epicycle> epicycles = fourierEngine.getEpicycles(time);

        // Limit to the number user wants to see
        if (epicycles.size() > static_cast<size_t>(numEpicyclesToShow)) {
            epicycles.resize(numEpicyclesToShow);
        }

        // Assign colors to epicycles - spread across visible ones
        for (size_t i = 0; i < epicycles.size(); i++) {
            // Use a better distribution that spreads colors across visible epicycles
            float t = static_cast<float>(i) / std::min(static_cast<int>(epicycles.size()), 5);
            t = std::min(t, 1.0f);  // Clamp to 1.0

            // Apply gradient: Pink -> Cyan -> Purple
            sf::Color color;
            if (t < 0.5f) {
                color = lerpColor(
                    sf::Color(255, 110, 199),  // Neon Pink
                    sf::Color(0, 240, 255),    // Cyan
                    t * 2.0f
                );
            } else {
                color = lerpColor(
                    sf::Color(0, 240, 255),    // Cyan
                    sf::Color(185, 103, 255),  // Purple
                    (t - 0.5f) * 2.0f
                );
            }

            epicycles[i].color = color;
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

        // Draw trail (if visible)
        if (showTrail) {
            renderer.drawTrail(window, trail);
        }

        // Draw user's drawn path if they're drawing
        if (inputHandler.isDrawing() && inputHandler.getDrawnPath().size() > 0) {
            renderer.drawUserPath(window, inputHandler.getDrawnPath());
        }

        // Draw epicycles (if visible)
        if (showEpicycles) {
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

                // Make connecting lines subtle
                sf::Color subtleColor = epicycles[i].color;
                subtleColor.a = 150;  // Add transparency

                // Draw connecting arm
                sf::Vertex line[2];
                line[0].position = lineStart.toSFML();
                line[0].color = subtleColor;
                line[1].position = lineEnd.toSFML();
                line[1].color = subtleColor;
                window.draw(line, 2, sf::PrimitiveType::Lines);

                lineStart = lineEnd;
            }

            // Draw epicycles
            renderer.drawEpicycles(window, epicycles);
        }

        // Draw glow at the drawing point
        if (!epicycles.empty()) {
            renderer.drawGlow(window, currentPos);
        }

        // Draw UI
        std::string speedText = "Speed: " + std::to_string(speed).substr(0, 3) + "x  (+/- to adjust)";
        uiManager.drawText(window, speedText, 10, 10);
        std::string epicycleText = "Epicycles: " + std::to_string(numEpicyclesToShow) + "  ([/] to adjust)";
        uiManager.drawText(window, epicycleText, 10, 30);
        uiManager.drawText(window, "Press 1-4: Shapes  |  Draw: Click & Drag  |  C: Clear  |  R: Reset  |  Space: Pause", 10, 690);

        // Display
        window.display();
    }

    return 0;
}
