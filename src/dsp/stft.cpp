#include "stft.h"
#include "fft.h"
#include <cmath>

std::vector<std::vector<double>> stftMagnitudeDb(
    const std::vector<cdouble>& x,
    int winSize,
    int hop,
    int nfft
){
    if (winSize <= 0 || hop <= 0 || nfft <= 0) return {};
    if (static_cast<int>(x.size()) < winSize) return {};

    std::vector<double> window(winSize);
    for (int i = 0; i < winSize; ++i) {
        window[i] = 0.5 - 0.5 * std::cos(2.0 * M_PI / (winSize - 1));
    }

    int numFrames = 1 + (static_cast<int>(x.size()) - winSize) / hop;
    int bins = nfft / 2;
    std::vector<std::vector<double>> spec;
    spec.reserve(numFrames);

    const double eps = 1e-12;

    for (int f = 0; f < numFrames; ++f){
        int start = f * hop;
        std::vector<cdouble> frame(nfft, {0.0, 0.0});
        for (int i = 0; i < winSize; ++i){
            frame[i] = x[start + i] * window[i];
        }

        auto X = fft(frame);
        std::vector<double> mags(bins, 0.0);
        for (int k = 0; k < bins; ++k){
            double mag = std::abs(X[k]);
            mags[k] = 20.0 * std::log10(mag + eps);
        }
        spec.push_back(std::move(mags));
    }
    return spec;
}