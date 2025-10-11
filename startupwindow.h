#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include "startupmodel.h"

class StartupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = nullptr);
    ~StartupWindow() = default;

private:
    QTableView *tableViewStartup;
    StartupModel *m_startupModel;
};

#endif // STARTUPWINDOW_H
