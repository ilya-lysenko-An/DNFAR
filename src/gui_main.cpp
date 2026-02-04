#include <QApplication>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "common/types.h"
#include "signal/signal_generator.h"
#include "dsp/beamforming.h"
#include "dsp/pattern.h"
#include "dsp/fft.h"

int main(int argc, char *argv[])
{
    std::cout << "Что показать?\n";
    std::cout << "1 - Диаграмма направленности\n";
    std::cout << "2 - Спектр\n";
    std::cout << "Введите номер: ";
    int mode = 0;
    std::cin >> mode;

    QApplication app(argc, argv);
    QMainWindow window;

    if (mode == 1) {
        int N = 8;
        double R = 28.0;
        double fc = 15e6;
        double steer_deg = 45.0;

        std::vector<double> antennaAzimuths;
        for (int i = 0; i < N; ++i) {
            antennaAzimuths.push_back(i * 360.0 / N);
        }

        auto weights = calculateWeights(R, fc, steer_deg, antennaAzimuths);

        int numAngles = 361;
        auto pattern = computeBeamPattern(weights, antennaAzimuths, R, fc, numAngles);

        QLineSeries *series = new QLineSeries();
        for (int k = 0; k < numAngles; ++k) {
            double angle = 360.0 * k / (numAngles - 1);
            series->append(angle, pattern[k]);
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Диаграмма направленности");
        chart->legend()->hide();

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("Угол (градусы)");
        axisX->setRange(0, 360);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Амплитуда");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        QChartView *view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);

        window.setCentralWidget(view);
        window.resize(900, 600);
        window.show();

        return app.exec();
    }

    if (mode == 2) {
        double angle1_deg = 0.0;
        double angle2_deg = 0.0;
        double steer_deg = 0.0;

        std::cout << "Введите угол прихода 1-го сигнала (градусы): ";
        std::cin >> angle1_deg;
        std::cout << "Введите угол прихода 2-го сигнала (градусы): ";
        std::cin >> angle2_deg;
        std::cout << "Введите угол фазирования ФАР (градусы): ";
        std::cin >> steer_deg;

        int N = 8;
        int L = 4096;
        double Fs = 100e6;
        double R = 28.0;
        double fc = 15e6;
        double SNR = 10.0;

        std::vector<Source> sources = {
            {14e6, angle1_deg, 1.0},
            {16e6, angle2_deg, 1.0}
        };

        auto antennSignals = generateAntennaSignals(sources, N, R, fc, Fs, L, SNR);

        std::vector<double> antennaAzimuths;
        for (int i = 0; i < N; ++i) {
            antennaAzimuths.push_back(i * 360.0 / N);
        }

        auto weights = calculateWeights(R, fc, steer_deg, antennaAzimuths);
        auto farSignal = beamformSignal(antennSignals, weights);

        auto spectrum = fft(farSignal);

        double maxMag = 0.0;
        for (int k = 0; k < L; ++k){
            maxMag = std::max(maxMag, std::abs(spectrum[k]));
        }

        QLineSeries *series = new QLineSeries();
        for (int k = 0; k < L / 2; ++k) {
            double freq = k * Fs / L; // Гц
            double mag = std::abs(spectrum[k]);
            double mag_db = 20.0 * std::log10((mag + 1e-12) / (maxMag + 1e-12));
            series->append(freq / 1e6, mag_db);
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Спектр сигнала");
        chart->legend()->hide();

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("Частота (МГц)");

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("|X|");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        QChartView *view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);

        window.setCentralWidget(view);
        window.resize(900, 600);
        window.show();

        return app.exec();
    }

    std::cout << "Неверный выбор\n";
    return 0;
}


