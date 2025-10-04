#include "InputHandler.h"

InputHandler::InputHandler() : drawing(false) {
}

void InputHandler::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // Handle mouse button pressed
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            drawing = true;
            drawnPath.clear();
        }
    }

    // Handle mouse button released
    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            drawing = false;
        }
    }

    // Handle mouse movement
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (drawing) {
            Point2D point(
                static_cast<float>(mouseMoved->position.x),
                static_cast<float>(mouseMoved->position.y)
            );
            drawnPath.push_back(point);
        }
    }
}

bool InputHandler::isDrawing() const {
    return drawing;
}

const std::vector<Point2D>& InputHandler::getDrawnPath() const {
    return drawnPath;
}

void InputHandler::clearPath() {
    drawnPath.clear();
}
