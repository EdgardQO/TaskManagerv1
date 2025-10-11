#include "historymanager.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include <QProcess> // <-- Añadido

HistoryManager::HistoryManager(QObject *parent) : QObject(parent)
{
    updateGuiPids(); // Actualizamos la lista de PIDs con GUI al inicio

    m_activeProcesses = getRunningProcesses();
    for (auto it = m_activeProcesses.constBegin(); it != m_activeProcesses.constEnd(); ++it) {
        HistoryEntry entry = it.value();
        entry.startTime = QDateTime::currentDateTime();
        entry.status = HistoryEntry::RUNNING;
        m_historyLog.append(entry);
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &HistoryManager::checkForChanges);
    connect(m_timer, &QTimer::timeout, this, &HistoryManager::updateGuiPids); // Conectamos la nueva función
    m_timer->start(2000);
}

const QList<HistoryEntry>& HistoryManager::getHistoryLog() const
{
    return m_historyLog;
}

void HistoryManager::checkForChanges()
{
    QMap<int, HistoryEntry> currentProcesses = getRunningProcesses();
    for (auto it = currentProcesses.constBegin(); it != currentProcesses.constEnd(); ++it) {
        if (!m_activeProcesses.contains(it.key())) {
            HistoryEntry newEntry = it.value();
            newEntry.startTime = QDateTime::currentDateTime();
            newEntry.status = HistoryEntry::RUNNING;
            m_historyLog.append(newEntry);
            m_activeProcesses.insert(it.key(), newEntry);
        }
    }
    QList<int> pidsToRemove;
    for (auto it = m_activeProcesses.begin(); it != m_activeProcesses.end(); ++it) {
        int pid = it.key();
        if (!currentProcesses.contains(pid)) {
            pidsToRemove.append(pid);
            for (int i = m_historyLog.size() - 1; i >= 0; --i) {
                if (m_historyLog[i].pid == pid && m_historyLog[i].status == HistoryEntry::RUNNING) {
                    m_historyLog[i].endTime = QDateTime::currentDateTime();
                    m_historyLog[i].status = HistoryEntry::FINISHED;
                    break;
                }
            }
        } else {
            it.value().memoryUsageKB = currentProcesses[pid].memoryUsageKB;
            it.value().cpuTimeSecs = currentProcesses[pid].cpuTimeSecs;
            for (int i = m_historyLog.size() - 1; i >= 0; --i) {
                if (m_historyLog[i].pid == pid && m_historyLog[i].status == HistoryEntry::RUNNING) {
                    m_historyLog[i].memoryUsageKB = currentProcesses[pid].memoryUsageKB;
                    m_historyLog[i].cpuTimeSecs = currentProcesses[pid].cpuTimeSecs;
                    break;
                }
            }
        }
    }
    for(int pid : pidsToRemove) {
        m_activeProcesses.remove(pid);
    }
    emit historyChanged();
}

// Nueva función que usa wmctrl para obtener los PIDs con ventana
void HistoryManager::updateGuiPids()
{
    QProcess process;
    process.start("wmctrl", QStringList() << "-lp");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    QSet<int> currentGuiPids;
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            int pid = parts[2].toInt();
            if (pid > 0) {
                currentGuiPids.insert(pid);
            }
        }
    }
    m_guiPids = currentGuiPids;
}

QMap<int, HistoryEntry> HistoryManager::getRunningProcesses()
{
    QMap<int, HistoryEntry> processes;
    const long ticksPerSecond = sysconf(_SC_CLK_TCK);
    const long pageSizeKB = sysconf(_SC_PAGESIZE) / 1024;

    QDir procDir("/proc");
    QStringList pidDirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &pidStr : pidDirs) {
        bool isNumeric;
        int pid = pidStr.toInt(&isNumeric);
        if (isNumeric) {

            // --- FILTRO FINAL: ¿ESTÁ ESTE PID EN NUESTRA LISTA DE VENTANAS? ---
            // 🚨 MODIFICACIÓN: Deshabilitamos el filtro para asegurar que aparezca data.
            // if (!m_guiPids.contains(pid)) {
            //     continue; // Si no tiene ventana, lo ignoramos.
            // }

            char exePath[1024];
            ssize_t len = readlink(QString("/proc/%1/exe").arg(pidStr).toUtf8().constData(), exePath, sizeof(exePath) - 1);
            if (len == -1) {
                continue;
            }

            QFile statFile(QString("/proc/%1/stat").arg(pidStr));
            if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString statLine = statFile.readLine();
                statFile.close();

                int firstParen = statLine.indexOf('(');
                int lastParen = statLine.lastIndexOf(')');
                if (firstParen == -1 || lastParen == -1) continue;

                HistoryEntry entry;
                entry.pid = pid;
                entry.name = statLine.mid(firstParen + 1, lastParen - firstParen - 1);

                QStringList stats = statLine.mid(lastParen + 2).split(' ');
                if (stats.size() >= 22) {
                    unsigned long utime = stats[11].toULong();
                    unsigned long stime = stats[12].toULong();
                    entry.cpuTimeSecs = static_cast<double>(utime + stime) / ticksPerSecond;

                    long rss_pages = stats[21].toLong();
                    entry.memoryUsageKB = rss_pages * pageSizeKB;

                    processes.insert(pid, entry);
                }
            }
        }
    }
    return processes;
}
