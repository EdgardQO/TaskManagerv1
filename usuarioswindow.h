#ifndef USUARIOSWINDOW_H
#define USUARIOSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QTimer>

class UsuariosWindow : public QWidget {
    Q_OBJECT

public:
    explicit UsuariosWindow(QWidget *parent = nullptr);
    ~UsuariosWindow();

private slots:
    // Mantiene el nombre de la lógica original para mayor claridad
    void actualizarProcesos();

private:
    QTableWidget *tabla;
    QTimer *timer;
};

#endif // USUARIOSWINDOW_H
