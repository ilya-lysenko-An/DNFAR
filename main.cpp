#include "dsp.h"
#include <iostream>
#include <vector>
#include <cmath>

int main() {
    // ====== 1. Источники сигнала ======
    std::vector<Source> sources = {
        {12e6, 90.0, 1.0},   // сигнал 12 МГц, азимут 90°
        {17e6, 140.0, 1.0}   // сигнал 17 МГц, азимут 140°
    };

    // ====== 2. Параметры ФАР ======
    int N = 8;          // число антенн
    int L = 4096;       // длина сигнала (отсчёты)
    double Fs = 50e6;   // частота дискретизации
    double R = 28.0;    // радиус ФАР
    double fc = 15e6;   // несущая частота
    double SNR = 10.0;  // отношение сигнал/шум

    // ====== 3. Генерация сигналов на всех антеннах ======
    auto signals = generateAntennaSignals(sources, N, R, fc, Fs, L, SNR);

    // ====== 4. Азимуты антенн ======
    std::vector<double> antennaAzimuths;
    for (int i = 0; i < N; ++i)
        antennaAzimuths.push_back(i * 360.0 / N);

    // ====== 5. Расчёт весовых коэффициентов ======
    double phi0 = 90.0; // направление главного максимума (азимут)
    auto weights = calculateWeights(R, fc, phi0, antennaAzimuths);

    // ====== 6. Формирование сигнала ФАР ======
    auto farSignal = beamformSignal(signals, weights);

    // ====== 7. FFT ======
    auto spectrum = fft(farSignal);

    // ====== 8. Вывод спектра ======
    std::cout << "=== Спектр сигнала после фазирования ===" << std::endl;
    for (size_t k = 0; k < L / 2; ++k) {
        double freq = k * Fs / L;
        double mag = std::abs(spectrum[k]);
        if (mag > 50.0) { // фильтр для наглядности
            std::cout << "f = " << freq / 1e6
                      << " MHz, |X| = " << mag << std::endl;
        }
    }

    return 0;
}
