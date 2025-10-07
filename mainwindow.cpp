#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Administrador de Tareas");

    connect(ui->actionProcesos,&QAction::triggered, this,&MainWindow::on_actionProcesos_triggered);
    connect(ui->actionRendimiento,&QAction::triggered, this,&MainWindow::on_actionRendimiento_triggered);

    ui->stackedWidgetPrincipal->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionProcesos_triggered()
{
    ui->stackedWidgetPrincipal->setCurrentIndex(0);
}
void MainWindow::on_actionRendimiento_triggered()
{
    ui->stackedWidgetPrincipal->setCurrentIndex(1);
}
