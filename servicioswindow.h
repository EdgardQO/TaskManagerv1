#ifndef SERVICIOSWINDOW_H
#define SERVICIOSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

class ServiciosWindow : public QWidget {
    Q_OBJECT

public:
    explicit ServiciosWindow(QWidget *parent = nullptr);
    ~ServiciosWindow() override = default;

private slots:
    void cargarServicios();
    void mostrarMenu(const QPoint &pos);
    void ejecutarAccion(const QString &accion, QString servicio = QString());
    void actualizarBotones();

private:
    QTableWidget *table;
    QLabel *statusLabel;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnRestart;
};

#endif // SERVICIOSWINDOW_H
