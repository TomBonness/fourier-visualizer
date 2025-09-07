#include "FourierEngine.h"
#include <cmath>
#include <algorithm>

const double TWO_PI = 2.0 * M_PI;

// Structure to store coefficient with its frequency index
struct CoeffData {
    std::complex<double> coeff;
    int frequency;
    double magnitude;
};

FourierEngine::FourierEngine() : numEpicycles(0), time(0.0) {
}

void FourierEngine::computeDFT(const std::vector<Point2D>& path) {
    originalPath = path;
    int N = path.size();

    if (N == 0) return;

    coefficients.clear();
    frequencies.clear();

    // Compute DFT coefficients for frequencies from -N/2 to N/2
    std::vector<CoeffData> coeffData;
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

        // Average and store coefficient with frequency
        std::complex<double> coeff = sum / static_cast<double>(N);
        coeffData.push_back({coeff, k, std::abs(coeff)});
    }

    // Sort by magnitude (largest first)
    std::sort(coeffData.begin(), coeffData.end(),
        [](const CoeffData& a, const CoeffData& b) {
            return a.magnitude > b.magnitude;
        });

    // Store sorted coefficients and frequencies
    for (const auto& data : coeffData) {
        coefficients.push_back(data.coeff);
        frequencies.push_back(data.frequency);
    }
}

std::vector<Epicycle> FourierEngine::getEpicycles(double t) const {
    std::vector<Epicycle> epicycles;

    if (coefficients.empty()) return epicycles;

    // Calculate epicycles for each frequency (already sorted by magnitude)
    for (int i = 0; i < static_cast<int>(coefficients.size()); i++) {
        int k = frequencies[i];  // Get stored frequency

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

    if (coefficients.empty()) return Point2D(0.0, 0.0);

    for (int i = 0; i < static_cast<int>(coefficients.size()); i++) {
        int k = frequencies[i];  // Get stored frequency
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
