#pragma once 
#include <vector>
#include "common/types.h"

std::vector<double> computeBeamPattern (
    const std::vector<cdouble>& weights,
    const std::vector<double>& antennaAzimuths_deg,
    double R,
    double fc,
    int N
);