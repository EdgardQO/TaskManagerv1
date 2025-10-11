#include "startupmodel.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QSettings> // Para leer archivos .desktop

StartupModel::StartupModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    fetchStartupEntries();
}

int StartupModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_startupList.count();
}

int StartupModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return 4; // Nombre, Editor, Estado, Impacto
}

QVariant StartupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

    switch (section) {
    case 0: return "Nombre";
    case 1: return "Editor";
    case 2: return "Estado";
    case 3: return "Grado de impacto";
    default: return QVariant();
    }
}

QVariant StartupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_startupList.size()) return QVariant();

    const StartupEntry &entry = m_startupList.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return entry.name;
        case 1: return entry.editor;
        case 2: return entry.status;
        case 3: return entry.impact;
        default: break;
        }
    }
    return QVariant();
}

void StartupModel::fetchStartupEntries()
{
    m_startupList.clear();

    // 1. Directorios de búsqueda de archivos .desktop
    QStringList autostartPaths;
    // Directorio del usuario (más común para aplicaciones añadidas por el usuario)
    autostartPaths << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";
    // Directorio del sistema (para aplicaciones instaladas)
    autostartPaths << "/etc/xdg/autostart";

    for (const QString &path : autostartPaths) {
        QDir dir(path);
        if (!dir.exists()) continue;

        QStringList entries = dir.entryList(QStringList() << "*.desktop", QDir::Files);

        for (const QString &fileName : entries) {
            QString filePath = dir.absoluteFilePath(fileName);
            QSettings desktopFile(filePath, QSettings::IniFormat);
            desktopFile.beginGroup("Desktop Entry");

            StartupEntry entry;
            entry.name = desktopFile.value("Name", fileName).toString();
            entry.command = desktopFile.value("Exec", "").toString();

            // Asumimos un editor simple si no se encuentra (puede requerir más lógica)
            entry.editor = desktopFile.value("Vendor", "Desconocido").toString();

            // 🚨 Lógica simple para el estado:
            // Si el archivo está presente, lo consideramos 'Habilitado'
            entry.status = "Habilitado";

            // 🚨 Lógica simple para impacto (Ejemplo, esto es estimación heurística):
            // Si la ruta contiene 'gnome', 'kde', 'system', 'update' -> Alto
            // Si contiene 'indicator', 'tray', 'daemon' -> Medio
            // En caso contrario -> Bajo
            QString lowerCommand = entry.command.toLower();
            if (lowerCommand.contains("update") || lowerCommand.contains("service") || lowerCommand.contains("kde") || lowerCommand.contains("gnome")) {
                entry.impact = "Alto";
            } else if (lowerCommand.contains("indicator") || lowerCommand.contains("tray") || lowerCommand.contains("daemon")) {
                entry.impact = "Medio";
            } else {
                entry.impact = "Bajo";
            }

            // Solo se añaden entradas válidas (con comando o nombre)
            if (!entry.command.isEmpty() || entry.name != fileName) {
                m_startupList.append(entry);
            }

            desktopFile.endGroup();
        }
    }

    // Notificar a la vista que el modelo se ha reiniciado
    beginResetModel();
    endResetModel();
}
