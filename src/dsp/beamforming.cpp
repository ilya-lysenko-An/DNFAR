#include "beamforming.h"
#include <cmath>

std::vector<cdouble> calculateWeights(
    double R,
    double fc,
    double phi0_deg,
    const std::vector<double>& antennaAzimuth
){
    std::vector<cdouble> W;
    double c = 3e8;
    double lambda = c / fc;
    double phi0_rad = phi0_deg * M_PI / 180.0;

    for (double alpha_deg : antennaAzimuth){
        double alpha_rad = alpha_deg * M_PI / 180.0;
        double phase = 2 * M_PI * R * cos(phi0_rad - alpha_rad) / lambda;
        W.push_back(std::exp(cdouble(0, +phase)));
    }
    return W;
}

std::vector<cdouble> beamformSignal (
    const std::vector<std::vector<cdouble>>& signals,
    const std::vector<cdouble>& weights
) {
    if (weights.size() != signals.size()) return {};
    int N = signals.size();
    if (N == 0) return {};
    int L = signals[0].size();

    std::vector<cdouble> result(L, 0);

    for (int n = 0; n < N; ++n ){
        for (int t = 0; t < L; ++t){
            result[t] += signals[n][t] * weights[n];
        }
    }
    return result;
}
