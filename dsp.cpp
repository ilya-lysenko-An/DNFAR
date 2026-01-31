#include "dsp.h"
#include <cmath>
#include <random>

static constexpr double C = 3e8;

static double gaussianNoise(double sigma) {
    static std::mt19937 gen(std::random_device{}());
    static std::normal_distribution<double> dist(0.0, 1.0);
    return sigma * dist(gen);
}

