#ifndef FOURIER_ENGINE_H
#define FOURIER_ENGINE_H

#include <vector>
#include <complex>
#include "Types.h"

class FourierEngine {
private:
    std::vector<std::complex<double>> coefficients;
    std::vector<int> frequencies;  // Corresponding frequency for each coefficient
    std::vector<Point2D> originalPath;
    int numEpicycles;
    double time;

public:
    FourierEngine();

    // Compute DFT from path points
    void computeDFT(const std::vector<Point2D>& path);

    // Get epicycle positions at current time
    std::vector<Epicycle> getEpicycles(double t) const;

    // Get the traced point at current time
    Point2D getTracedPoint(double t) const;

    // Update time and animation state
    void update(double dt, double speed);

    // Setters
    void setNumEpicycles(int n);
};

#endif // FOURIER_ENGINE_H
