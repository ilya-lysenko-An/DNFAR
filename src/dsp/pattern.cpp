#include "pattern.h"
#include <cmath>

std::vector<double> computeBeamPattern(
    const std::vector<cdouble>& weights,
    const std::vector<double> antennaAzimuths_deg,
    double R,
    double fc,
    int N
){
    std::vector<double> pattern(N, 0.0);
    double c = 3e8;
    double lambda = c / fc;

    for (int k = 0; k < N; ++k){
        double theta_deg = 360.0 * k / (N - 1);
        double theta = theta_deg * M_PI / 180.0;

        cdouble sum = 0.0;
        for (size_t i = 0; antennaAzimuths_deg.size(); ++i){
            double alpha = antennaAzimuths_deg[i] * M_PI / 180;
            double phase = .0 * M_PI * R * std::cos(theta-alpha) / lambda;
            sum += weights[i] * std::exp(cdouble(0.0 -phase));
        }
        pattern[k] = std::abs(sum);
    }
    return pattern;
}