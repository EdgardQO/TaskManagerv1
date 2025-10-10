#ifndef PROCESOSWINDOW_H
#define PROCESOSWINDOW_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QTimer>
#include <QHeaderView>
#include "processtablemodel.h"

class ProcesosWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProcesosWindow(QWidget *parent = nullptr);
    ~ProcesosWindow() = default;

private:
    QTableView *tableViewProcesos;
    ProcessTableModel *m_processModel;
    QTimer *m_timer;
    void on_tableViewProcesos_customContextMenuRequested(const QPoint &pos);
    void terminateProcess(); // Slot para la acción "Finalizar Tarea"
};

#endif // PROCESOSWINDOW_H
