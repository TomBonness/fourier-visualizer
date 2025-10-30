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

std::vector<Point2D> PathData::resamplePath(const std::vector<Point2D>& path, int targetPoints) {
    // TODO: Implement resampling
    return path;
}

std::vector<Point2D> PathData::centerPath(const std::vector<Point2D>& path, Point2D center) {
    // TODO: Implement centering
    return path;
}
