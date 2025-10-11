#include "historymodel.h"
#include <QtMath> // Necesario para qRound

HistoryModel::HistoryModel(HistoryManager *manager, QObject *parent)
    : QAbstractTableModel(parent), m_manager(manager)
{
    connect(m_manager, &HistoryManager::historyChanged, this, &HistoryModel::handleHistoryChange);
}

void HistoryModel::handleHistoryChange()
{
    // Una actualización completa de la tabla es lo más sencillo
    beginResetModel();
    endResetModel();
}

int HistoryModel::rowCount(const QModelIndex &parent) const
{
    return m_manager->getHistoryLog().count();
}

int HistoryModel::columnCount(const QModelIndex &parent) const
{
    return 6; // 6 columnas
}

QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }
    switch (section) {
    case 0: return "Aplicaciones";
    case 1: return "PID";
    case 2: return "Memoria (MB)"; // Mostrar en MB
    case 3: return "Tiempo CPU (s)";
    case 4: return "Hora de Inicio";
    case 5: return "Hora de Fin";
    default: return QVariant();
    }
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const QList<HistoryEntry> &log = m_manager->getHistoryLog();
    // Invertimos la fila porque el modelo muestra el historial más reciente arriba
    const HistoryEntry &entry = log.at(log.count() - 1 - index.row());

    // 🚨 CORRECCIÓN: Conversión explícita a (int) para Qt::TextAlignmentRole
    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 1: // PID
        case 2: // Memoria
        case 3: // Tiempo CPU
            // Devolver la combinación de banderas convertida a int
            return (int)(Qt::AlignRight | Qt::AlignVCenter);
        case 4: // Inicio
        case 5: // Fin
            return (int)Qt::AlignCenter;
        default:
            // Aplicaciones
            return (int)(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (index.column()) {
    case 0:
    {
        // Limpiar el nombre de la aplicación
        QString name = entry.name;
        int lastSlash = name.lastIndexOf('/');
        if (lastSlash != -1) {
            name = name.mid(lastSlash + 1);
        }
        return name;
    }
    case 1: return entry.pid;
    case 2:
    {
        // Mostrar en MB y redondear
        double memoryMB = static_cast<double>(entry.memoryUsageKB) / 1024.0;
        return QString::number(qRound(memoryMB));
    }
    case 3:
    {
        // Formatear el tiempo CPU con dos decimales
        return QString::number(entry.cpuTimeSecs, 'f', 2);
    }
    case 4: return entry.startTime.toString("hh:mm:ss");
    case 5:
        return (entry.status == HistoryEntry::FINISHED) ? entry.endTime.toString("hh:mm:ss") : "En ejecución";
    default:
        return QVariant();
    }
}
