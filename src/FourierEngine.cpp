#include "FourierEngine.h"
#include <cmath>

const double TWO_PI = 2.0 * M_PI;

FourierEngine::FourierEngine() : numEpicycles(0), time(0.0) {
}

void FourierEngine::computeDFT(const std::vector<Point2D>& path) {
    originalPath = path;
    int N = path.size();

    if (N == 0) return;

    coefficients.clear();

    // Compute DFT coefficients for frequencies from -N/2 to N/2
    for (int k = -N/2; k < N/2; k++) {
        std::complex<double> sum(0.0, 0.0);

        for (int n = 0; n < N; n++) {
            // Convert point to complex number
            std::complex<double> point(path[n].x, path[n].y);

            // Calculate e^(-i*2π*k*n/N)
            double angle = -TWO_PI * k * n / N;
            std::complex<double> exponential(std::cos(angle), std::sin(angle));

            // Add to sum
            sum += point * exponential;
        }

        // Average and store coefficient
        coefficients.push_back(sum / static_cast<double>(N));
    }
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
