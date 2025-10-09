#include "processtablemodel.h"
#include <QRegularExpression>
#include <QStringView>
#include <QStringList>
#include <QRegularExpression>
#include <algorithm>



// =======================================================
// MÉTODOS REQUERIDOS DEL MODELO
// =======================================================

ProcessTableModel::ProcessTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    fetchLinuxProcesses(); // Carga inicial para establecer valores previos
}

int ProcessTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_processList.count();
}

int ProcessTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return 3; // Columnas: Nombre, % CPU, PID
}

QVariant ProcessTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

    switch (section) {
    case 0: return "Nombre del Proceso";
    case 1: {
        QString header = QString("CPU Total: %1%").arg(QString::number(m_totalCpuUsage,'f',1));
        return header;
    };
    case 2: return "PID";
    default: return QVariant();
    }
}

QVariant ProcessTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_processList.size()) return QVariant();

    const ProcessInfo &info = m_processList.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return info.name;
        case 1: return QString::number(info.cpu_usage, 'f', 1); // Porcentaje con 1 decimal
        case 2: return info.pid;
        default: break;
        }
    }
    return QVariant();
}

// =======================================================
// LÓGICA DE OBTENCIÓN DE PROCESOS (Linux)
// =======================================================

void ProcessTableModel::fetchLinuxProcesses()
{
    // Lógica para obtener el tiempo total de CPU (desde /proc/stat)
    QFile statFile("/proc/stat");
    qulonglong totalCPUTime = 0;
    if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&statFile);
        QString line = in.readLine();
        statFile.close();

        if (line.startsWith("cpu ")) {
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            for (int i = 1; i < parts.size(); ++i) {
                totalCPUTime += parts.at(i).toULongLong();
            }
        }
    }

    QList<ProcessInfo> newProcessList;
    QMap<int, qulonglong> currentCPUTime;
    qulonglong totalTimeDelta = totalCPUTime - m_prevTotalCPUTime;

    // Iterar sobre el directorio /proc
    QDir procDir("/proc");
    QStringList pidDirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &pidStr : pidDirs) {
        bool ok;
        int pid = pidStr.toInt(&ok);
        if (!ok) continue;

        QFile statProcFile("/proc/" + pidStr + "/stat");
        if (statProcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString line = statProcFile.readAll();
            statProcFile.close();

            int nameStart = line.indexOf('(') + 1;
            int nameEnd = line.lastIndexOf(')');
            if (nameStart > 0 && nameEnd > nameStart) {
                ProcessInfo info;
                info.pid = pid;
                info.name = line.mid(nameStart, nameEnd - nameStart);

                QString fullLine(line);
                QString remainder = fullLine.mid(nameEnd+2);
                QStringList fields = remainder.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

                if (fields.size() >= 14) {
                    qulonglong utime = fields[11].toULongLong();
                    qulonglong stime = fields[12].toULongLong();
                    qulonglong processTime = utime + stime;
                    currentCPUTime[pid] = processTime;

                    double cpu_usage = 0.0;
                    if (m_prevCPUTime.contains(pid) && totalTimeDelta > 0) {
                        qulonglong timeDelta = processTime - m_prevCPUTime.value(pid);
                        cpu_usage = (double)timeDelta * 100.0 / totalTimeDelta;
                    }
                    info.cpu_usage = qMin(cpu_usage, 100.0);
                    newProcessList.append(info);
                }
            }
        }
    }

    // Actualizar estado
    m_processList = newProcessList;
    // 🚨 CÁLCULO DEL TOTAL DE CPU UTILIZADO POR TODOS LOS PROCESOS
    double totalCpu = 0.0;
    for (const ProcessInfo &info : newProcessList) {
        totalCpu += info.cpu_usage;
    }
    // NOTA: En sistemas Linux con N núcleos, el uso de CPU puede superar el 100%.
    // Simplemente sumamos el uso por cada proceso.
    m_totalCpuUsage = totalCpu;
    // -----------------------------------------
    m_prevCPUTime = currentCPUTime;
    m_prevTotalCPUTime = totalCPUTime;
}

void ProcessTableModel::updateProcessList()
{
    beginResetModel();
    fetchLinuxProcesses();
    sort(m_sortColumn,m_sortOrder);
    endResetModel();
}

// processtablemodel.cpp (Implementación de sort)

// processtablemodel.cpp (Implementación de sort)

// processtablemodel.cpp

void ProcessTableModel::sort(int column, Qt::SortOrder order)
{
    // 🚨 1. GUARDA EL ESTADO DE ORDENACIÓN (CORRECTO)
    m_sortColumn = column;
    m_sortOrder = order;

    // 2. Lógica de Ordenación:
    std::sort(m_processList.begin(), m_processList.end(),
              // ... (Tu lambda de comparación, que es correcta)
              [column, order](const ProcessInfo &a, const ProcessInfo &b) {
                  // ...
                  return false;
              });

    // 🚨 3. ELIMINAR: NO LLAMAR A begin/endResetModel AQUÍ
}
