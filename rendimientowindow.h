#ifndef RENDIMIENTOWINDOW_H
#define RENDIMIENTOWINDOW_H

#include <QWidget>
#include "cpuwidget.h"
#include "recursos.h"

class RendimientoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RendimientoWindow(QWidget *parent = nullptr);
    ~RendimientoWindow() override = default;
};

#endif // RENDIMIENTOWINDOW_H
