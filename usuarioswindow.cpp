#include "usuarioswindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QProcess>
#include <QStringList>
#include <QDebug> // Opcional para depuración

UsuariosWindow::UsuariosWindow(QWidget *parent) : QWidget(parent) {
    tabla = new QTableWidget(this);
    tabla->setColumnCount(5);
    // Encabezados copiados de usuariorepo
    tabla->setHorizontalHeaderLabels({"PID", "Usuario", "%CPU", "%Memoria", "Comando"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tabla);

    timer = new QTimer(this);
    // Conexión y temporizador de usuariorepo
    connect(timer, &QTimer::timeout, this, &UsuariosWindow::actualizarProcesos);
    timer->start(2000); // Actualiza cada 2 segundos

    actualizarProcesos();
}

UsuariosWindow::~UsuariosWindow() {}

void UsuariosWindow::actualizarProcesos() {
    QProcess proceso;
    // Comando 'ps' de usuariorepo
    proceso.start("ps", QStringList() << "-eo" << "pid,user,pcpu,pmem,comm");

    // El método de usuariorepo es usar waitForFinished
    proceso.waitForFinished();
    QString salida = proceso.readAllStandardOutput();

    QStringList lineas = salida.split('\n', Qt::SkipEmptyParts);
    // Se resta 1 por la línea de encabezado del comando 'ps'
    tabla->setRowCount(lineas.size() - 1);

    for (int i = 1; i < lineas.size(); ++i) {
        QString linea = lineas[i].simplified();
        QStringList columnas = linea.split(' ', Qt::SkipEmptyParts);

        for (int j = 0; j < columnas.size() && j < 5; ++j) {
            tabla->setItem(i - 1, j, new QTableWidgetItem(columnas[j]));
        }
    }
}
