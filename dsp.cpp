#include "dsp.h"
#include <cmath>
#include <random>

static constexpr double C = 3e8; // скорость света, м/с

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

    // Оценка мощности сигнала 
    double signalPower = 1.0;
    double noiseSigma = std::sqrt(signalPower / std::pow(10.0, SNR / 10.0));

    // Основной цикл по времени
    for (int n = 0; n < L; ++n) {
        double t = n / Fs;

        // По антеннам
        for (int i = 0; i < N; ++i) {
            cdouble sample = 0.0;

            // Сумма вкладов всех источников
            for (const auto& src : sources) {
                double phi_src = src.azimuth * M_PI / 180.0;

                // Фазовая задержка из-за геометрии ФАР
                double phaseDelay =
                    2.0 * M_PI * src.freq *
                    (R * std::cos(phi_src - antennaAzimuths[i]) / C);

                // Сигнал на несущей
                double phase =
                    2.0 * M_PI * (fc + src.freq) * t - phaseDelay;

                sample += src.amplitude * std::polar(1.0, phase);
            }

            // Добавляем AWGN
            sample += cdouble(
                gaussianNoise(noiseSigma),
                gaussianNoise(noiseSigma)
            );

            signals[i][n] = sample;
        }
    }

    return signals;
}

std::vector<cdouble> downconvert(
    const std::vector<double>& signal,
    double fc,
    double Fs
)
{
    int L = signal.size();
    std::vector<cdouble> iq(L);

    for (int n = 0; n < L; ++n) {
        double t = n / Fs;
        double phase = -2.0 * M_PI * fc * t;

        cdouble lo = std::polar(1.0, phase);
        iq[n] = signal[n] * lo;
    }

    return iq;
}

std::vector<cdouble> fft(const std::vector<cdouble>& x)
{
    int N = x.size();
    std::vector<cdouble> X(N);

    for (int k = 0; k < N; ++k) {
        cdouble sum = 0.0;
        for (int n = 0; n < N; ++n) {
            double angle = -2.0 * M_PI * k * n / N;
            sum += x[n] * std::polar(1.0, angle);
        }
        X[k] = sum;
    }

    return X;
}



