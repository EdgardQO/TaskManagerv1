#include "detalleswindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QProcess>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <pwd.h>
#include <signal.h>
#include <dirent.h>
#include <algorithm> // Para std::replace

// Inclusiones de C necesarias para la lectura de /proc
extern "C" {
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
}

DetallesWindow::DetallesWindow(QWidget *parent) : QWidget(parent) {
    // Adaptación a QWidget
    QVBoxLayout *layout = new QVBoxLayout(this);

    tabla = new QTableWidget(this);
    tabla->setColumnCount(7);
    tabla->setHorizontalHeaderLabels({"PID", "Usuario", "Nombre", "Estado", "CPU %", "Memoria (MB)", "Comando"});
    tabla->horizontalHeader()->setStretchLastSection(true);
    // Configuración de selección para el botón
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);


    botonFinalizar = new QPushButton("Finalizar proceso");
    connect(botonFinalizar, &QPushButton::clicked, this, &DetallesWindow::finalizarProceso);

    layout->addWidget(tabla);
    layout->addWidget(botonFinalizar);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DetallesWindow::actualizarProcesos);
    timer->start(3000); // Actualizar cada 3 segundos

    actualizarProcesos();
}

void DetallesWindow::actualizarProcesos() {
    tabla->setRowCount(0);
    cargarProcesos();
}

void DetallesWindow::cargarProcesos() {
    DIR *dir = opendir("/proc");
    if (!dir) return;

    struct dirent *ent;
    while ((ent = readdir(dir)) != nullptr) {
        if (!isdigit(*ent->d_name)) continue;

        int pid = atoi(ent->d_name);
        std::string path = "/proc/" + std::string(ent->d_name);
        std::ifstream statFile(path + "/stat");
        std::ifstream statusFile(path + "/status");
        std::ifstream cmdFile(path + "/cmdline");

        if (!statFile || !statusFile) continue;

        std::string name, state;
        double cpuUsage = 0.0;
        long memoryKB = 0;

        // Leer nombre y estado
        statFile >> pid >> name >> state;

        // Leer memoria y usuario
        std::string line;
        std::string uidStr;
        while (std::getline(statusFile, line)) {
            if (line.rfind("Uid:", 0) == 0) {
                std::istringstream uidStream(line.substr(5));
                uidStream >> uidStr;
            } else if (line.rfind("VmRSS:", 0) == 0) {
                std::istringstream mem(line.substr(6));
                mem >> memoryKB;
            }
        }

        // Obtener nombre de usuario
        struct passwd *pw = getpwuid(std::stoul(uidStr));
        std::string username = pw ? pw->pw_name : "N/A";

        // Comando
        std::string cmd;
        std::getline(cmdFile, cmd);
        if (cmd.empty()) cmd = "[" + name + "]";
        // Reemplazar nulos en cmdline con espacios
        std::replace(cmd.begin(), cmd.end(), '\0', ' ');

        // Agregar fila
        int row = tabla->rowCount();
        tabla->insertRow(row);

        tabla->setItem(row, 0, new QTableWidgetItem(QString::number(pid)));
        tabla->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(username)));
        tabla->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(name).remove('(').remove(')')));
        tabla->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(state)));
        tabla->setItem(row, 4, new QTableWidgetItem("N/A"));
        tabla->setItem(row, 5, new QTableWidgetItem(QString::number(memoryKB / 1024)));
        tabla->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(cmd)));
    }
    closedir(dir);
}

void DetallesWindow::finalizarProceso() {
    QList<QTableWidgetItem *> seleccion = tabla->selectedItems();
    if (seleccion.isEmpty()) {
        QMessageBox::warning(this, "Atención", "Selecciona un proceso para finalizar.");
        return;
    }

    int fila = seleccion.first()->row();
    int pid = tabla->item(fila, 0)->text().toInt();
    QString processName = tabla->item(fila, 2)->text();

    if (pid <= 0) {
        QMessageBox::critical(this, "Error", "PID inválido.");
        return;
    }

    // Usar pkexec para garantizar permisos y solicitar clave
    QProcess process;
    process.start("pkexec", QStringList() << "kill" << "-9" << QString::number(pid));
    process.waitForFinished();

    int exitCode = process.exitCode();
    QString standardError = process.readAllStandardError();

    if (exitCode == 0) {
        QMessageBox::information(this, "Éxito", QString("Proceso '%1' (PID %2) finalizado correctamente.").arg(processName).arg(pid));
    } else {
        QString errorMsg = QString("No se pudo finalizar el proceso '%1' (PID %2).\n"
                                   "Código de salida: %3.\n"
                                   "Error del sistema: %4").arg(processName).arg(pid).arg(exitCode).arg(standardError.trimmed());
        QMessageBox::critical(this, "Error", errorMsg);
    }

    actualizarProcesos();
}
