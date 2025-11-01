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

        points.push_back(Point2D(x * scale, -y * scale));  // Flip y to make heart right-side up
    }

    return points;
}

std::vector<Point2D> PathData::createStar(int numPoints, int spikes, float scale) {
    std::vector<Point2D> points;

    float outerRadius = scale;
    float innerRadius = scale * 0.4f;  // Inner points are 40% of outer radius

    // Distribute points around the star shape
    for (int i = 0; i < numPoints; i++) {
        float t = static_cast<float>(i) / numPoints;
        float angle = t * 2.0f * M_PI - M_PI / 2;  // Start from top

        // Figure out which spike we're on
        float spikePosition = t * spikes;
        float withinSpike = spikePosition - std::floor(spikePosition);

        // Interpolate radius smoothly between outer and inner points
        // 0 = outer point, 0.5 = inner point, 1.0 = next outer point
        float radiusFactor;
        if (withinSpike < 0.5f) {
            radiusFactor = 1.0f - withinSpike * 2.0f;  // Go from outer to inner
        } else {
            radiusFactor = (withinSpike - 0.5f) * 2.0f;  // Go from inner to outer
        }

        float radius = innerRadius + (outerRadius - innerRadius) * radiusFactor;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);

        points.push_back(Point2D(x, y));
    }

    return points;
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

std::vector<Point2D> PathData::createInfinity(int numPoints, float scale) {
    std::vector<Point2D> points;

    // Lemniscate of Gerono (infinity symbol / figure-8)
    // Parametric equations: x = cos(t), y = sin(t) * cos(t)
    for (int i = 0; i < numPoints; i++) {
        float t = (2.0f * M_PI * i) / numPoints;

        float x = scale * std::cos(t);
        float y = scale * std::sin(t) * std::cos(t);

        points.push_back(Point2D(x, y));
    }

    return points;
}

std::vector<Point2D> PathData::resamplePath(const std::vector<Point2D>& path, int targetPoints) {
    if (path.size() < 2) return path;

    // Calculate total path length
    float totalLength = 0.0f;
    for (size_t i = 1; i < path.size(); i++) {
        float dx = path[i].x - path[i-1].x;
        float dy = path[i].y - path[i-1].y;
        totalLength += std::sqrt(dx * dx + dy * dy);
    }

    // Resample at even intervals
    std::vector<Point2D> resampled;
    float targetDistance = totalLength / (targetPoints - 1);

    resampled.push_back(path[0]);

    float accumulatedDist = 0.0f;
    size_t currentIdx = 0;

    for (int i = 1; i < targetPoints - 1; i++) {
        float targetDist = i * targetDistance;

        // Find the segment containing this target distance
        while (currentIdx < path.size() - 1) {
            float dx = path[currentIdx + 1].x - path[currentIdx].x;
            float dy = path[currentIdx + 1].y - path[currentIdx].y;
            float segmentLength = std::sqrt(dx * dx + dy * dy);

            if (accumulatedDist + segmentLength >= targetDist) {
                // Interpolate within this segment
                float t = (targetDist - accumulatedDist) / segmentLength;
                Point2D interpolated(
                    path[currentIdx].x + t * dx,
                    path[currentIdx].y + t * dy
                );
                resampled.push_back(interpolated);
                break;
            }

            accumulatedDist += segmentLength;
            currentIdx++;
        }
    }

    resampled.push_back(path[path.size() - 1]);
    return resampled;
}

std::vector<Point2D> PathData::centerPath(const std::vector<Point2D>& path, Point2D center) {
    if (path.empty()) return path;

    // Find current center (centroid)
    float sumX = 0.0f, sumY = 0.0f;
    for (const auto& p : path) {
        sumX += p.x;
        sumY += p.y;
    }
    Point2D currentCenter(sumX / path.size(), sumY / path.size());

    // Translate to new center
    std::vector<Point2D> centered;
    float dx = center.x - currentCenter.x;
    float dy = center.y - currentCenter.y;

    for (const auto& p : path) {
        centered.push_back(Point2D(p.x + dx, p.y + dy));
    }

    return centered;
}
