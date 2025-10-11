#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QDateTime>
#include <QString>
#include <QMap>
#include <QSet> // <-- Añadido

// La struct HistoryEntry no cambia
struct HistoryEntry {
    enum Status { RUNNING, FINISHED };

    int pid;
    QString name;
    QDateTime startTime;
    QDateTime endTime;
    Status status;

    long memoryUsageKB = 0;
    double cpuTimeSecs = 0.0;
};

class HistoryManager : public QObject
{
    Q_OBJECT

public:
    explicit HistoryManager(QObject *parent = nullptr);
    const QList<HistoryEntry>& getHistoryLog() const;

signals:
    void historyChanged();

private slots:
    void checkForChanges();
    void updateGuiPids(); // <-- Nuevo slot

private:
    QMap<int, HistoryEntry> getRunningProcesses();
    QMap<int, HistoryEntry> m_activeProcesses;
    QTimer *m_timer;
    QList<HistoryEntry> m_historyLog;
    QSet<int> m_guiPids; // <-- Nuevo Set para PIDs con ventana
};

#endif // HISTORYMANAGER_H
