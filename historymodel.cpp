#include "historymodel.h"

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
    case 2: return "Memoria (KB)";
    case 3: return "Tiempo CPU (s)";
    case 4: return "Hora de Inicio";
    case 5: return "Hora de Fin";
    default: return QVariant();
    }
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    const HistoryEntry &entry = m_manager->getHistoryLog().at(rowCount() - 1 - index.row());

    switch (index.column()) {
    case 0: return entry.name;
    case 1: return entry.pid;
    case 2: return QString::number(entry.memoryUsageKB);
    case 3: return QString::number(entry.cpuTimeSecs, 'f', 2);
    case 4: return entry.startTime.toString("hh:mm:ss");
    case 5:
        return (entry.status == HistoryEntry::FINISHED) ? entry.endTime.toString("hh:mm:ss") : "En ejecución";
    default:
        return QVariant();
    }
}
