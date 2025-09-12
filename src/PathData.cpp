#include "PathData.h"
#include <cmath>

std::vector<Point2D> PathData::createCircle(int numPoints, float radius) {
    std::vector<Point2D> points;

    for (int i = 0; i < numPoints; i++) {
        float angle = (2.0f * M_PI * i) / numPoints;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        points.push_back(Point2D(x, y));
    }

    return points;
}

std::vector<Point2D> PathData::createHeart(int numPoints, float scale) {
    std::vector<Point2D> points;

    for (int i = 0; i < numPoints; i++) {
        float t = (2.0f * M_PI * i) / numPoints - M_PI;  // Start from -π to π

        // Parametric heart equation
        float x = 16.0f * std::pow(std::sin(t), 3);
        float y = 13.0f * std::cos(t) - 5.0f * std::cos(2.0f * t)
                  - 2.0f * std::cos(3.0f * t) - std::cos(4.0f * t);

        points.push_back(Point2D(x * scale, y * scale));  // y positive is down in SFML
    }

    return points;
}

std::vector<Point2D> PathData::createStar(int numPoints, int spikes, float scale) {
    // TODO: Implement star shape
    return createCircle(numPoints, scale);
}

std::vector<Point2D> PathData::createSquare(int numPoints, float size) {
    std::vector<Point2D> points;
    float halfSize = size / 2.0f;

    // Distribute points evenly around the square perimeter
    int pointsPerSide = numPoints / 4;

    // Top side (left to right)
    for (int i = 0; i < pointsPerSide; i++) {
        float t = static_cast<float>(i) / pointsPerSide;
        float x = -halfSize + t * size;
        float y = -halfSize;
        points.push_back(Point2D(x, y));
    }

    // Right side (top to bottom)
    for (int i = 0; i < pointsPerSide; i++) {
        float t = static_cast<float>(i) / pointsPerSide;
        float x = halfSize;
        float y = -halfSize + t * size;
        points.push_back(Point2D(x, y));
    }

    // Bottom side (right to left)
    for (int i = 0; i < pointsPerSide; i++) {
        float t = static_cast<float>(i) / pointsPerSide;
        float x = halfSize - t * size;
        float y = halfSize;
        points.push_back(Point2D(x, y));
    }

    // Left side (bottom to top)
    for (int i = 0; i < pointsPerSide; i++) {
        float t = static_cast<float>(i) / pointsPerSide;
        float x = -halfSize;
        float y = halfSize - t * size;
        points.push_back(Point2D(x, y));
    }

    return points;
}

std::vector<Point2D> PathData::resamplePath(const std::vector<Point2D>& path, int targetPoints) {
    // TODO: Implement resampling
    return path;
}

std::vector<Point2D> PathData::centerPath(const std::vector<Point2D>& path, Point2D center) {
    // TODO: Implement centering
    return path;
}
