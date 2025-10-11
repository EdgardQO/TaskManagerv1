#ifndef HISTORIALWINDOW_H
#define HISTORIALWINDOW_H

#include <QWidget>
#include <QTableView>
#include "historymanager.h" // 🚨 Se asume que estos archivos se agregan al proyecto
#include "historymodel.h"   // 🚨 Se asume que estos archivos se agregan al proyecto

class HistorialWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistorialWindow(QWidget *parent = nullptr);
    ~HistorialWindow() override = default;

private:
    HistoryManager *m_historyManager;
    HistoryModel *m_historyModel;
    QTableView *m_tableView;
};

#endif // HISTORIALWINDOW_H
