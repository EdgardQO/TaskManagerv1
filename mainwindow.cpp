#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "procesoswindow.h"
#include <QVBoxLayout> // Necesario para el layout

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

    // =======================================================
    // 2. CONEXIONES Y VISTA INICIAL
    // =======================================================

    // Conexiones manuales (la lógica de navegación no cambia)
    connect(ui->actionProcesos, &QAction::triggered,
            this, &MainWindow::on_actionProcesos_triggered);
    connect(ui->actionRendimiento, &QAction::triggered,
            this, &MainWindow::on_actionRendimiento_triggered);

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
