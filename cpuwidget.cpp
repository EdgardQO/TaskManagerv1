// cpuwidget.cpp
#include "cpuwidget.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <thread>
#include <algorithm>
#include <QDebug>
#include <QPen>
#include <QFont> // ✅ Necesitamos incluir QFont

CPUWidget::CPUWidget(QWidget *parent)
    : QWidget(parent),
    chart(new QChart()),
    chartView(new QChartView(chart)),
    axisX(new QValueAxis()),
    axisY(new QValueAxis()),
    timer(new QTimer(this)),
    windowSec(60),
    cores(0)
{
    // 1️⃣ Detectar núcleos
    cores = contarCores();
    if (cores <= 0) cores = std::max(1u, std::thread::hardware_concurrency());
    cores = std::clamp(cores, 1, 16);

    cpuDataBuffer.resize(cores);

    // 2️⃣ Crear series (líneas suaves)
    series.reserve(cores);
    for (int i = 0; i < cores; ++i) {
        auto *s = new QSplineSeries();
        s->setName(QString("CPU %1").arg(i + 1));
        chart->addSeries(s);

        QPen pen = s->pen();
        pen.setWidthF(1.8);
        s->setPen(pen);

        series.push_back(s);
    }

    // 3️⃣ Configurar ejes
    axisX->setRange(0, windowSec - 1);
    axisX->setReverse(true);
    axisX->setTickCount(7);
    axisX->setLabelFormat("%d s");
    axisX->setTitleText("Tiempo");

    axisY->setRange(0, 100);
    axisY->setTitleText("Uso (%)");
    axisY->setLabelFormat("%.0f %%");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for (auto *s : series) {
        s->attachAxis(axisX);
        s->attachAxis(axisY);
    }

    // 4️⃣ Estilo y Layout
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // ✅ SOLUCIÓN FINAL: Usamos una fuente monoespaciada para la leyenda.
    // Esto asegura que todos los caracteres tengan el mismo ancho y elimina el "baile".
    QFont legendFont("Monospace");
    legendFont.setStyleHint(QFont::Monospace);
    chart->legend()->setFont(legendFont);

    chart->setTitle("Uso de CPU por Núcleo");
    chartView->setRenderHint(QPainter::Antialiasing);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);

    // 5️⃣ Leer estado inicial
    QVector<quint64> initialTotals, initialIdles;
    if(leerProcStat(initialTotals, initialIdles)) {
        prevTotal = initialTotals;
        prevIdle = initialIdles;
    }

    // 6️⃣ Iniciar temporizador
    connect(timer, &QTimer::timeout, this, &CPUWidget::actualizarCPU);
    timer->start(1000);
}

// ... (El resto del archivo, contarCores, leerProcStat y actualizarCPU, se mantiene exactamente igual que en la versión anterior) ...

int CPUWidget::contarCores() const {
    QFile statFile("/proc/stat");
    if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;
    QTextStream in(&statFile);
    int coreCount = 0;
    in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("cpu") && line.length() > 3 && line.at(3).isDigit()) {
            coreCount++;
        } else {
            break;
        }
    }
    return coreCount;
}

bool CPUWidget::leerProcStat(QVector<quint64>& totals, QVector<quint64>& idles) const {
    QFile statFile("/proc/stat");
    if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    totals.clear();
    idles.clear();
    QTextStream in(&statFile);
    in.readLine();
    int coresToRead = cores;
    int coresRead = 0;
    while (!in.atEnd() && coresRead < coresToRead) {
        QString line = in.readLine();
        if (line.startsWith("cpu") && line.length() > 3 && line.at(3).isDigit()) {
            QStringList values = line.split(' ', Qt::SkipEmptyParts);
            if (values.size() < 8) continue;
            quint64 user = values[1].toULongLong(), nice = values[2].toULongLong(), system = values[3].toULongLong(),
                idle = values[4].toULongLong(), iowait = values[5].toULongLong(), irq = values[6].toULongLong(),
                softirq = values[7].toULongLong();
            quint64 totalTime = user + nice + system + idle + iowait + irq + softirq;
            quint64 idleTime = idle + iowait;
            totals.append(totalTime);
            idles.append(idleTime);
            coresRead++;
        } else {
            break;
        }
    }
    return !totals.isEmpty();
}

void CPUWidget::actualizarCPU() {
    QVector<quint64> currentTotals, currentIdles;
    if (!leerProcStat(currentTotals, currentIdles)) return;

    if (prevTotal.isEmpty() || currentTotals.size() != prevTotal.size()) {
        prevTotal = currentTotals;
        prevIdle = currentIdles;
        return;
    }

    const int loopLimit = std::min((int)cores, (int)currentTotals.size());

    for (int i = 0; i < loopLimit; ++i) {
        quint64 totalDiff = currentTotals[i] - prevTotal[i];
        quint64 idleDiff = currentIdles[i] - prevIdle[i];

        double usage = 0.0;
        if (totalDiff > 0 && totalDiff >= idleDiff) {
            usage = 100.0 * (double)(totalDiff - idleDiff) / (double)totalDiff;
        }

        // Mantenemos la lógica de justificación, que ahora funcionará perfectamente con la fuente monoespaciada.
        QString usageText = QString("%1%").arg(QLocale().toString(usage, 'f', 1));
        QString legendText = QString("CPU %1 %2")
                                 .arg(i + 1)
                                 .arg(usageText.rightJustified(6, ' '));
        series[i]->setName(legendText);

        cpuDataBuffer[i].append(usage);
        if (cpuDataBuffer[i].size() > windowSec) {
            cpuDataBuffer[i].removeFirst();
        }

        QVector<QPointF> points;
        points.reserve(cpuDataBuffer[i].size());

        for(int j = 0; j < cpuDataBuffer[i].size(); ++j) {
            points.append(QPointF(j, cpuDataBuffer[i].at(cpuDataBuffer[i].size() - 1 - j)));
        }
        series[i]->replace(points);
    }

    prevTotal = currentTotals;
    prevIdle = currentIdles;
}
