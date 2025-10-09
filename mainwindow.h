#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "procesoswindow.h" // ¡Incluir!


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

private:
    Ui::MainWindow *ui;
    int m_procesosIndex = 0; // Añade esta variable de miembro

};
#endif // MAINWINDOW_H
