#pragma once
#include <vector>
#include "common/types.h"

std::vector<std::vector<cdouble>> generateAntennaSignals(
    const std::vector<Source>& sources,
    int N,
    double R,
    double fc,
    double Fs,
    int L,
    double SNR
);
