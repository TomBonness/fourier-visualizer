#include "Renderer.h"
#include <algorithm>
#include <cmath>

Renderer::Renderer() {
}

sf::Color Renderer::lerpColor(sf::Color a, sf::Color b, float t) {
    return sf::Color(
        static_cast<uint8_t>(a.r + (b.r - a.r) * t),
        static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t)
    );
}

void Renderer::drawTrail(sf::RenderWindow& window, const std::vector<Point2D>& trail) {
    if (trail.size() > 1) {
        for (size_t i = 1; i < trail.size(); i++) {
            // Quadratic fade for smoother effect
            float t = static_cast<float>(i) / trail.size();
            float fade = t * t;  // Quadratic easing
            int alpha = static_cast<int>(fade * 255 * 1.6f);  // Bolder trail
            alpha = std::min(alpha, 255);

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
}

void Renderer::drawEpicycles(sf::RenderWindow& window, const std::vector<Epicycle>& epicycles) {
    // Draw all epicycles
    for (const auto& epic : epicycles) {
        // Make epicycles more subtle with transparency
        sf::Color subtleColor = epic.color;
        subtleColor.a = 150;  // Add transparency

        // Draw the circle
        sf::CircleShape circle(epic.radius);
        circle.setOrigin({epic.radius, epic.radius});
        circle.setPosition(epic.center.toSFML());
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(1.5f);  // Subtle outline
        circle.setOutlineColor(subtleColor);
        window.draw(circle);

        // Draw center dot (also subtle)
        sf::CircleShape dot(2.f);  // Slightly smaller
        dot.setOrigin({2.f, 2.f});
        dot.setPosition(epic.center.toSFML());
        dot.setFillColor(subtleColor);
        window.draw(dot);
    }
}

void Renderer::drawGlow(sf::RenderWindow& window, const Point2D& position) {
    // Multi-layer glow effect
    for (int i = 5; i >= 1; i--) {
        float radius = i * 3.0f;
        int alpha = 50 / i;  // Fade outward

        sf::CircleShape glow(radius);
        glow.setOrigin({radius, radius});
        glow.setPosition(position.toSFML());
        glow.setFillColor(sf::Color(0, 240, 255, alpha));  // Cyan glow
        window.draw(glow, sf::BlendAdd);  // Additive blending for glow
    }

    // Bright center point
    sf::CircleShape centerPoint(2.f);
    centerPoint.setOrigin({2.f, 2.f});
    centerPoint.setPosition(position.toSFML());
    centerPoint.setFillColor(sf::Color(255, 255, 255));
    window.draw(centerPoint, sf::BlendAdd);
}
