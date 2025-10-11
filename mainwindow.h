#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "procesoswindow.h"
#include "startupwindow.h" // 🚨 Incluir la nueva ventana

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionProcesos_triggered();
    void on_actionRendimiento_triggered();
    // 🚨 Slot para la Page 4
    void on_actionInicio_triggered();

private:
    Ui::MainWindow *ui;
    int m_procesosIndex = 0;

    // Si usas el índice 4 para "Inicio", no necesitas más miembros aquí.
};
#endif // MAINWINDOW_H
