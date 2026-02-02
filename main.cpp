#include "dsp.h"
#include <iostream>
#include <cmath>

int main() {
    std::vector<Source> sources = {
        {12e6, 90.0, 1.0},
        {17e6, 140.0, 1.0}
    };

    int N = 8;
    int L = 4096 ;
    double Fs = 50e6;

    auto signals = generateAntennaSignals(
        sources,
        N,
        28.0,
        15e6,
        Fs,
        L,
        10.0
    );
    auto& x = signals[0];

    auto X = fft(x);

    for (int k = 0; k < L / 2; ++k){
        double freq = k * Fs / L;
        double mag = std::abs(X[k]);
        if (mag > 50.0) { // порог для наглядности
            std::cout << "f = " << freq / 1e6
                      << " MHz, |X| = " << mag << std::endl;
    }

}
return 0;
}