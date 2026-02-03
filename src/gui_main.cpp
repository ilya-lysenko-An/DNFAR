#include <QApplication>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "common/types.h"
#include "dsp/beamforming.h"
#include "dsp/pattern.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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

    QMainWindow window;
    window.setCentralWidget(view);
    window.resize(900, 600);
    window.show();

    return app.exec();
}

