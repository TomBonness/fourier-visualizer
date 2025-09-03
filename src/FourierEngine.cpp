#include "FourierEngine.h"
#include <cmath>

FourierEngine::FourierEngine() : numEpicycles(0), time(0.0) {
}

void FourierEngine::computeDFT(const std::vector<Point2D>& path) {
    // TODO: Implement DFT computation
    originalPath = path;
}

std::vector<Epicycle> FourierEngine::getEpicycles(double t) const {
    // TODO: Calculate epicycle positions
    std::vector<Epicycle> epicycles;
    return epicycles;
}

Point2D FourierEngine::getTracedPoint(double t) const {
    // TODO: Calculate the final traced point
    return Point2D(0.0, 0.0);
}

void FourierEngine::update(double dt, double speed) {
    time += dt * speed;
}

void FourierEngine::setNumEpicycles(int n) {
    numEpicycles = n;
}
