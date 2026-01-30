#include "dsp.h"

int main() {
    std::vector<Source> sources = {
        {12e6, 90.0, 1.0},
        {17e6, 140.0, 1.0}
    };

    auto signals = generateAntennaSignals(sources, 8, 28.0, 15e6, 50e6, 4096, 10.0);

}