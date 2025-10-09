#include "procesoswindow.h"

ProcesosWindow::ProcesosWindow(QWidget *parent)
    : QWidget(parent)
{
    // 1. Inicializar componentes
    m_processModel = new ProcessTableModel(this);
    tableViewProcesos = new QTableView(this);

    // 2. Configurar la tabla
    tableViewProcesos->setModel(m_processModel);
    tableViewProcesos->setSortingEnabled(true);

    // Ajuste de columnas
    tableViewProcesos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableViewProcesos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableViewProcesos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);


    //orden
    tableViewProcesos->sortByColumn(0,Qt::AscendingOrder);


    // 3. Configurar el layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableViewProcesos);
    setLayout(mainLayout);

    // 4. Configurar e iniciar el temporizador
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, m_processModel, &ProcessTableModel::updateProcessList);
    m_timer->start(1000); // 1 segundo
}
