#ifndef PATH_DATA_H
#define PATH_DATA_H

#include <vector>
#include "Types.h"

class PathData {
public:
    // Generate a circle path
    static std::vector<Point2D> createCircle(int numPoints, float radius = 100.f);

    // Utility: resample path to have a specific number of points
    static std::vector<Point2D> resamplePath(const std::vector<Point2D>& path, int targetPoints);

    // Utility: center a path around a specific point
    static std::vector<Point2D> centerPath(const std::vector<Point2D>& path, Point2D center);
};

#endif // PATH_DATA_H
