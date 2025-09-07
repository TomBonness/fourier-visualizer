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
    std::vector<Epicycle> epicycles;

    int N = originalPath.size();
    if (N == 0 || coefficients.empty()) return epicycles;

    // Calculate epicycles for each frequency
    for (int i = 0; i < static_cast<int>(coefficients.size()); i++) {
        int k = i - N/2;  // Frequency index

        // Get the coefficient for this frequency
        std::complex<double> coeff = coefficients[i];

        // Calculate rotation: coeff * e^(i*2π*k*t)
        double angle = TWO_PI * k * t;
        std::complex<double> rotation(std::cos(angle), std::sin(angle));
        std::complex<double> rotated = coeff * rotation;

        // Create epicycle
        Epicycle epic;
        epic.radius = std::abs(coeff);  // Magnitude
        epic.frequency = k;
        epic.phase = std::arg(coeff);   // Phase
        epic.center = Point2D(rotated.real(), rotated.imag());
        epic.color = sf::Color::White;  // Will be set later

        epicycles.push_back(epic);
    }

    return epicycles;
}

Point2D FourierEngine::getTracedPoint(double t) const {
    // The traced point is the sum of all rotated coefficients
    std::complex<double> sum(0.0, 0.0);

    int N = originalPath.size();
    if (N == 0 || coefficients.empty()) return Point2D(0.0, 0.0);

    for (int i = 0; i < static_cast<int>(coefficients.size()); i++) {
        int k = i - N/2;
        double angle = TWO_PI * k * t;
        std::complex<double> rotation(std::cos(angle), std::sin(angle));
        sum += coefficients[i] * rotation;
    }

    return Point2D(sum.real(), sum.imag());
}

void FourierEngine::update(double dt, double speed) {
    time += dt * speed;
}

void FourierEngine::setNumEpicycles(int n) {
    numEpicycles = n;
}
