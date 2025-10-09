#ifndef PROCESSTABLEMODEL_H
#define PROCESSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include <QDebug>

struct ProcessInfo {
    int pid;
    QString name;
    double cpu_usage;
};

class ProcessTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProcessTableModel(QObject *parent = nullptr);

    // Métodos obligatorios del Modelo/Vista
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //metodo sort
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
    void updateProcessList();

private:
    QList<ProcessInfo> m_processList;

    // 🚨 AÑADE ESTOS MIEMBROS PARA ALMACENAR EL ESTADO DE ORDENACIÓN
    int m_sortColumn = 0;
    Qt::SortOrder m_sortOrder = Qt::AscendingOrder;
    // ---------------------------------------------------------------

    //sumar total CPU
    double m_totalCpuUsage = 0.0;


    QMap<int, qulonglong> m_prevCPUTime;
    qulonglong m_prevTotalCPUTime = 0;

    void fetchLinuxProcesses();
};

#endif // PROCESSTABLEMODEL_H
