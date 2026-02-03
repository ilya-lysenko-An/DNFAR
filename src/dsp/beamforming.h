#pragma once
#include <vector>
#include "common/types.h"

std::vector<cdouble> calculateWeights(
    double R,
    double fc,
    double phi0_deg,
    const std::vector<double>& antennaAzimuth
);

std::vector<cdouble> beamformSignal(
    const std::vector<std::vector<cdouble>>& signals_in,
    const std::vector<cdouble>& weights
);
