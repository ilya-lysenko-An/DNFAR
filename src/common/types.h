#pragma once
#include <complex>

using cdouble = std::complex<double>;

struct Source {
    double freq;
    double azimuth;
    double amplitude;
};
