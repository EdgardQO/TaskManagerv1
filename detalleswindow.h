#ifndef DETALLESWINDOW_H
#define DETALLESWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QTimer>
#include <QPushButton>

class DetallesWindow : public QWidget {
    Q_OBJECT

public:
    explicit DetallesWindow(QWidget *parent = nullptr);
    ~DetallesWindow() override = default;

private slots:
    void actualizarProcesos();
    void finalizarProceso();

private:
    QTableWidget *tabla;
    QTimer *timer;
    QPushButton *botonFinalizar;
    void cargarProcesos();
};

#endif // DETALLESWINDOW_H
