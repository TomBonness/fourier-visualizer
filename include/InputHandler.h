#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Types.h"

class InputHandler {
public:
    InputHandler();

    // Handle events
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    // Query state
    bool isDrawing() const;
    const std::vector<Point2D>& getDrawnPath() const;

    // Clear the drawn path
    void clearPath();

private:
    bool drawing;
    std::vector<Point2D> drawnPath;
};

#endif // INPUT_HANDLER_H
