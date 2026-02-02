#pragma once
#include <vector>
#include <complex>

using cdouble = std::complex<double>;

// Структура источника сигнала
struct Source {
    double freq;     // частота сигнала (Гц)
    double azimuth;  // угол прихода (градусы)
    double amplitude;// амплитуда
};

// Генерация сигналов на всех элементах ФАР
std::vector<std::vector<cdouble>> generateAntennaSignals(
    const std::vector<Source>& sources,
    int N,            // число антенн
    double R,         // радиус ФАР (м)
    double fc,        // несущая частота (Гц)
    double Fs,        // частота дискретизации (Гц)
    int L,            // длина сигнала (отсчёты)
    double SNR        // сигнал/шум
);

// IQ downconversion (для одного канала)
std::vector<cdouble> downconvert(const std::vector<double>& signal, double fc, double Fs);

// FFT (базовая, можно позже заменить на библиотеку)
std::vector<cdouble> fft(const std::vector<cdouble>& x);

std::vector<cdouble> beamformSignal(
    const std::vector<std::vector<cdouble>>& signals, // [N][L]
    const std::vector<cdouble>& weights               // Wi 
);
