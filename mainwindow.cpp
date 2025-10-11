#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "procesoswindow.h"
#include "startupwindow.h" // 🚨 Incluir la nueva ventana
#include <QVBoxLayout> // Necesario para el layout
#include "usuarioswindow.h" // 🚨 Incluir la nueva ventana
#include "servicioswindow.h" // 🚨 Incluir la nueva ventana
#include "historialwindow.h" // 🚨 Incluir la nueva ventana de Historial




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Administrador de Tareas");

    // =======================================================
    // 1. INTEGRACIÓN DEL WIDGET DE PROCESOS (DENTRO DEL CONTENEDOR EXISTENTE)
    // =======================================================

    // a. OBTENER EL WIDGET CONTENEDOR ORIGINAL para Procesos (Índice 0)
    // Este es el widget 'Page' que creaste en el diseñador.
    QWidget *procesosContainer = ui->stackedWidgetPrincipal->widget(0);

    // b. Crear la instancia de la página de procesos, usando el contenedor como padre
    // Es CRUCIAL que el contenedor sea el padre para que ocupe ese espacio.
    ProcesosWindow *procesosView = new ProcesosWindow(procesosContainer);

    // c. ESTABLECER UN LAYOUT en el contenedor original y añadir la vista
    // Esto asegura que procesosView ocupe todo el espacio de Page 0.
    QVBoxLayout *layout = new QVBoxLayout(procesosContainer);
    layout->setContentsMargins(0, 0, 0, 0); // (Opcional: elimina los márgenes)
    layout->addWidget(procesosView);

    // d. IMPORTANTE: Hace visible el widget hijo
    procesosView->show();

    QWidget *startupContainer = ui->stackedWidgetPrincipal->widget(3);

    if (startupContainer) {
        // b. Crear la instancia de la página de inicio
        StartupWindow *startupView = new StartupWindow(startupContainer);

        // c. ESTABLECER UN LAYOUT en el contenedor original y añadir la vista
        QVBoxLayout *startupLayout = new QVBoxLayout(startupContainer);
        startupLayout->setContentsMargins(0, 0, 0, 0);
        startupLayout->addWidget(startupView);

        // d. Hace visible el widget hijo
        startupView->show();
    }

    // =======================================================
    // 🚨 3. INTEGRACIÓN DEL WIDGET DE USUARIOS (NUEVA PÁGINA: Índice 4)
    // =======================================================
    // Obtener el widget contenedor para Usuarios (Índice 4)
    QWidget *usuariosContainer = ui->stackedWidgetPrincipal->widget(4);

    if (usuariosContainer) {
        // a. Crear la instancia de la página de usuarios
        UsuariosWindow *usuariosView = new UsuariosWindow(usuariosContainer);

        // b. ESTABLECER UN LAYOUT en el contenedor original y añadir la vista
        QVBoxLayout *usuariosLayout = new QVBoxLayout(usuariosContainer);
        usuariosLayout->setContentsMargins(0, 0, 0, 0);
        usuariosLayout->addWidget(usuariosView);

        // c. Hace visible el widget hijo
        usuariosView->show();
    }


    QWidget *serviciosContainer = ui->stackedWidgetPrincipal->widget(6);

    if (serviciosContainer) {
        // a. Crear la instancia de la página de servicios
        ServiciosWindow *serviciosView = new ServiciosWindow(serviciosContainer);

        // b. ESTABLECER UN LAYOUT en el contenedor original y añadir la vista
        QVBoxLayout *serviciosLayout = new QVBoxLayout(serviciosContainer);
        serviciosLayout->setContentsMargins(0, 0, 0, 0);
        serviciosLayout->addWidget(serviciosView);

        // c. Hace visible el widget hijo
        serviciosView->show();
    }

    QWidget *detallesContainer = ui->stackedWidgetPrincipal->widget(5);

    if (detallesContainer) {
        // a. Crear la instancia de la página de detalles
        DetallesWindow *detallesView = new DetallesWindow(detallesContainer);

        // b. ESTABLECER UN LAYOUT en el contenedor original y añadir la vista
        QVBoxLayout *detallesLayout = new QVBoxLayout(detallesContainer);
        detallesLayout->setContentsMargins(0, 0, 0, 0);
        detallesLayout->addWidget(detallesView);

        // c. Hace visible el widget hijo
        detallesView->show();
    }


    QWidget *historialContainer = ui->stackedWidgetPrincipal->widget(2);

    if (historialContainer) {
        // a. Crear la instancia de la página de historial
        HistorialWindow *historialView = new HistorialWindow(historialContainer);

        // b. ESTABLECER UN LAYOUT en el contenedor original y añadir la vista
        QVBoxLayout *historialLayout = new QVBoxLayout(historialContainer);
        historialLayout->setContentsMargins(0, 0, 0, 0);
        historialLayout->addWidget(historialView);

        // c. Hace visible el widget hijo
        historialView->show();
    }

    // =======================================================
    // 2. CONEXIONES Y VISTA INICIAL
    // =======================================================

    // Conexiones manuales (la lógica de navegación no cambia)
    connect(ui->actionProcesos, &QAction::triggered,
            this, &MainWindow::on_actionProcesos_triggered);
    connect(ui->actionRendimiento, &QAction::triggered,
            this, &MainWindow::on_actionRendimiento_triggered);
    // 🚨 Conexión para la acción de inicio
    connect(ui->actionInicio, &QAction::triggered,
            this, &MainWindow::on_actionInicio_triggered);
    // 🚨 Conexión para la acción de Usuarios
    connect(ui->actionUsuarios, &QAction::triggered,
            this, &MainWindow::on_actionUsuarios_triggered);
    // 🚨 Conexión para la acción de Servicios
    connect(ui->actionServicios, &QAction::triggered,
            this, &MainWindow::on_actionServicios_triggered);
    // Conexión para la acción de Detalles
    connect(ui->actionDetalles, &QAction::triggered,
            this, &MainWindow::on_actionDetalles_triggered);
    connect(ui->actionHistorial, &QAction::triggered,
            this, &MainWindow::on_actionHistorial_triggered);

    // VISTA INICIAL: Establece la Page 0 (Procesos)
    ui->stackedWidgetPrincipal->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionProcesos_triggered()
{
    // Carga la Page 0 (ahora con el contenido de ProcesosWindow)
    ui->stackedWidgetPrincipal->setCurrentIndex(0);
}

void MainWindow::on_actionRendimiento_triggered()
{
    // Carga la Page 1 (Rendimiento)
    ui->stackedWidgetPrincipal->setCurrentIndex(1);
}
// 🚨 Implementación del nuevo slot para Inicio
void MainWindow::on_actionInicio_triggered()
{
    ui->stackedWidgetPrincipal->setCurrentIndex(3);
}
// 🚨 Implementación del nuevo slot para Usuarios
void MainWindow::on_actionUsuarios_triggered()
{
    // Carga la Page 4 (Usuarios)
    ui->stackedWidgetPrincipal->setCurrentIndex(4);
}
// 🚨 Implementación del nuevo slot para Servicios
void MainWindow::on_actionServicios_triggered()
{
    // Carga la Page 6 (Servicios)
    ui->stackedWidgetPrincipal->setCurrentIndex(6);
}
// 🚨 Implementación del nuevo slot para Detalles
void MainWindow::on_actionDetalles_triggered()
{
    // Carga la Page 5 (Detalles)
    ui->stackedWidgetPrincipal->setCurrentIndex(5);
}
// 🚨 Implementación del nuevo slot para Historial
void MainWindow::on_actionHistorial_triggered()
{
    // Carga la Page 2 (Historial)
    ui->stackedWidgetPrincipal->setCurrentIndex(2);
}
