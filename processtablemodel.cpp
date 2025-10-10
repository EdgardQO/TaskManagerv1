#include "processtablemodel.h"
#include <QRegularExpression>
#include <QStringView>
#include <QStringList>
#include <QRegularExpression>
#include <algorithm>

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
    return 4; // Columnas: Nombre, % CPU, PID
}

QVariant ProcessTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

    switch (section) {
    case 0: return "Nombre del Proceso";
    case 1: { return QString("CPU Total: %1%").arg(QString::number(m_totalCpuUsage, 'f', 1)); }

    case 2: { // 🚨 COLUMNA DE MEMORIA: Total en GB y Porcentaje

        // Convertir KB a GB (1024 * 1024 = 1048576)
        double totalGB = (double)m_usedProcessMemoryKB / 1048576.0;

        // Calcular el porcentaje total del sistema
        double usagePercent = 0.0;
        if (m_totalSystemMemoryKB > 0) {
            usagePercent = (double)m_usedProcessMemoryKB * 100.0 / m_totalSystemMemoryKB;
        }

        // Formato final: "Memoria Total: X.X GB (Y.Y%)"
        QString header = QString("Memoria Total: %1 GB (%2%)")
                             .arg(QString::number(totalGB, 'f', 1)) // 1 decimal para GB
                             .arg(QString::number(usagePercent, 'f', 1)); // 1 decimal para %
        return header;
    }

    case 3: return "PID";
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
        case 1: return QString::number(info.cpu_usage, 'f', 1);
        case 2: {
            // 🚨 Memoria individual en MB con 1 decimal
            double usageMB = (double)info.memory_usage_kb / 1024.0;
            return QString("%1 MB").arg(QString::number(usageMB, 'f', 1));
        }
        case 3: return info.pid;
        default: break;
        }
    }
    // ... (Tu lógica existente para Qt::SortRole con info.memory_usage_kb)
    return QVariant();
}

// =======================================================
// LÓGICA DE OBTENCIÓN DE PROCESOS (Linux)
// =======================================================

void ProcessTableModel::fetchLinuxProcesses()
{
    // --- 0. Inicializar variables para el total ---
    long long currentUsedProcessMemory = 0;
    long long memTotal = 0;
    long long memAvailable = 0;

    // --- 1. Obtener el tiempo total de CPU (/proc/stat) ---
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

    // --- 2. OBTENER DATOS DE MEMORIA (/proc/meminfo) ---
    QFile meminfoFile("/proc/meminfo");
    if (meminfoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&meminfoFile);
        QString line;
        while (in.readLineInto(&line)) {
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                if (parts[0] == "MemTotal:") {
                    memTotal = parts[1].toLongLong();
                } else if (parts[0] == "MemAvailable:") {
                    memAvailable = parts[1].toLongLong();
                }
            }
        }
        meminfoFile.close();
    }

    // 🚨 CÁLCULO CRUCIAL: Uso de Memoria del Sistema (como lo ve Ubuntu)
    if (memTotal > 0 && memAvailable > 0) {
        m_totalSystemMemoryKB = memTotal; // Total de RAM (el denominador)
        // La memoria usada por el sistema es el Total menos lo Disponible.
        m_usedProcessMemoryKB = memTotal - memAvailable;
    } else {
        // Fallback si no se pudo leer MemTotal o MemAvailable
        m_totalSystemMemoryKB = 0;
        m_usedProcessMemoryKB = 0;
    }


    QList<ProcessInfo> newProcessList;
    QMap<int, qulonglong> currentCPUTime;
    qulonglong totalTimeDelta = totalCPUTime - m_prevTotalCPUTime;

    // --- 3. Iterar sobre /proc para obtener datos individuales (VmRSS) ---
    QDir procDir("/proc");
    QStringList pidDirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &pidStr : pidDirs) {
        bool ok;
        int pid = pidStr.toInt(&ok);
        if (!ok) continue;

        ProcessInfo info;
        info.pid = pid;
        long long vmrss = 0;

        // a. Leer /proc/[pid]/stat para CPU y Nombre
        QFile statProcFile("/proc/" + pidStr + "/stat");
        if (statProcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString line = statProcFile.readAll();
            statProcFile.close();
            // ... (Tu lógica existente para calcular CPU y asignar a info.cpu_usage) ...

            int nameStart = line.indexOf('(') + 1;
            int nameEnd = line.lastIndexOf(')');
            if (nameStart > 0 && nameEnd > nameStart) {
                info.name = line.mid(nameStart, nameEnd - nameStart);

                QString fullLine(line);
                QString remainder = fullLine.mid(nameEnd + 2);
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
                }
            }
        }

        // b. Leer /proc/[pid]/status para Memoria (VmRSS)
        QFile statusFile("/proc/" + pidStr + "/status");
        if (statusFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream statusIn(&statusFile);
            QString statusLine;
            while (statusIn.readLineInto(&statusLine)) {
                if (statusLine.startsWith("VmRSS:")) {
                    QStringList parts = statusLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                    if (parts.size() >= 2) {
                        vmrss = parts[1].toLongLong(); // VmRSS está en KB
                        break;
                    }
                }
            }
            statusFile.close();
            info.memory_usage_kb = vmrss;
        }

        // Si logramos leer el nombre, lo añadimos a la lista
        if (!info.name.isEmpty()) {
            newProcessList.append(info);
        }
    }

    // --- 4. ALMACENAR ESTADO Y TOTALES ---

    // a. CPU Total (se suma el uso individual, no cambia)
    double totalCpu = 0.0;
    for (const ProcessInfo &info : newProcessList) {
        totalCpu += info.cpu_usage;
    }
    m_totalCpuUsage = totalCpu;

    // b. Actualizar estado del modelo
    m_processList = newProcessList;
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
