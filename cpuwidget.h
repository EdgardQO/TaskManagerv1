// cpuwidget.h
#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>

// ✅ CORRECCIÓN: Usamos QSplineSeries para líneas suaves.
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

class QTimer;

class CPUWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CPUWidget(QWidget *parent = nullptr);
    ~CPUWidget() override = default;

private slots:
    void actualizarCPU();

private:
    int contarCores() const;
    bool leerProcStat(QVector<quint64>& totals, QVector<quint64>& idles) const;

    // Componentes de la gráfica
    QChart* chart;
    QChartView* chartView;
    // ✅ CORRECCIÓN: El vector ahora contiene punteros a QSplineSeries.
    QVector<QSplineSeries*> series;
    QValueAxis* axisX;
    QValueAxis* axisY;

    // Lógica de actualización
    QTimer* timer;
    int windowSec;
    int cores;
    QVector<quint64> prevTotal, prevIdle;
    QVector<QList<double>> cpuDataBuffer;
};

#endif // CPUWIDGET_H
