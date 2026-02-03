#include "signal_generator.h"
#include <cmath>
#include <random>

static constexpr double C = 3e8;

static double gaussianNoise(double sigma)
{
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
){
    std::vector<std::vector<cdouble>> signals(
        N, std::vector<cdouble>(L, {0.0, 0.0})
    );

    std::vector<double> antennaAzimuths(N);
    for (int i = 0; i < N; ++i) {
        antennaAzimuths[i] = 2.0 * M_PI * i / N;
    }

    double signalPower = 1.0;
    double noiseSigma = std::sqrt(signalPower / std::pow(10.0, SNR / 10.0));

    for (int n = 0; n < L; ++n) {
        double t = n / Fs;

        for (int i = 0; i < N; ++i) {
            cdouble sample = 0.0;

            for (const auto& src : sources) {
                double phi_src = src.azimuth * M_PI / 180.0;

                double phaseDelay =
                    2.0 * M_PI * src.freq *
                    (R * std::cos(phi_src - antennaAzimuths[i]) / C);

                double phase =
                    2.0 * M_PI * src.freq * t - phaseDelay;

                sample += src.amplitude * std::polar(1.0, phase);
            }

            sample += cdouble(
                gaussianNoise(noiseSigma),
                gaussianNoise(noiseSigma)
            );

            signals[i][n] = sample;
        }
    }

    return signals;
}
