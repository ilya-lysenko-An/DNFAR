#pragma once 
#include <vector>
#include "common/types.h"

std::vector<std::vector<double>> stftMagnitudeDb(
    const std::vector<cdouble>& x,
    int winSize,
    int hop,
    int nfft
);