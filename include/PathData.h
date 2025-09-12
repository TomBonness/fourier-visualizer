#ifndef PATH_DATA_H
#define PATH_DATA_H

#include <vector>
#include "Types.h"

class PathData {
public:
    // Generate a circle path
    static std::vector<Point2D> createCircle(int numPoints, float radius = 100.f);

    // Generate a heart path
    static std::vector<Point2D> createHeart(int numPoints, float scale = 10.f);

    // Generate a star path
    static std::vector<Point2D> createStar(int numPoints, int spikes = 5, float scale = 100.f);

    // Generate a square path
    static std::vector<Point2D> createSquare(int numPoints, float size = 100.f);

    // Utility: resample path to have a specific number of points
    static std::vector<Point2D> resamplePath(const std::vector<Point2D>& path, int targetPoints);

    // Utility: center a path around a specific point
    static std::vector<Point2D> centerPath(const std::vector<Point2D>& path, Point2D center);
};

#endif // PATH_DATA_H
