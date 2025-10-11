#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "procesoswindow.h"
#include "startupwindow.h" // 🚨 Incluir la nueva ventana
#include "usuarioswindow.h" // 🚨 Incluir la nueva ventana de Usuarios
#include "servicioswindow.h" // 🚨 Incluir la nueva ventana de Servicios
#include "detalleswindow.h" // 🚨 Incluir la nueva ventana de Detalles
#include "historialwindow.h" // 🚨 Incluir la nueva ventana de Historial




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

    void on_actionUsuarios_triggered();
    void on_actionServicios_triggered(); // 🚨 Nuevo Slot para la Page 6 (Servicios)
    void on_actionDetalles_triggered(); // 🚨 Nuevo Slot para la Page 5 (Detalles)
    void on_actionHistorial_triggered(); // 🚨 Nuevo Slot para la Page 2 (Historial)




private:
    Ui::MainWindow *ui;
    int m_procesosIndex = 0;


    void highlightActiveAction(int index);

};
#endif // MAINWINDOW_H
