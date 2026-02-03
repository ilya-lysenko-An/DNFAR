#include "fft.h"
#include <cmath>

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
