#include "dsp.h"
#include <cmath>
#include <random>

static constexpr double C = 3e8;

static double gaussianNoise(double sigma) {
    static std::mt19937 gen(std::random_device{}());
    static std::normal_distribution<double> dist(0.0, 1.0);
    return sigma * dist(gen);
}

std::vector<std::vector<cdouble>> generateAntennaSignals(
    const std::vector<Source>& sources,
    int N,
    double R,
    double fc,
    double Fs,
    int L,
    double SNR
) {
        std::vector<std::vector<cdouble>> signals(
        N, std::vector<cdouble>(L, {0.0, 0.0})
    );
}

