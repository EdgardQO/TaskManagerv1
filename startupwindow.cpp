#include "startupwindow.h"
#include <QHeaderView>

StartupWindow::StartupWindow(QWidget *parent)
    : QWidget(parent)
{
    // 1. Inicializar componentes
    m_startupModel = new StartupModel(this);
    tableViewStartup = new QTableView(this);

    // 2. Configurar la tabla
    tableViewStartup->setModel(m_startupModel);
    tableViewStartup->setSortingEnabled(true);
    tableViewStartup->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableViewStartup->setSelectionMode(QAbstractItemView::SingleSelection);

    // Ajuste de columnas
    tableViewStartup->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // Nombre (Columna 0) toma el resto del espacio
    tableViewStartup->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableViewStartup->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableViewStartup->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // 3. Configurar el layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableViewStartup);
    setLayout(mainLayout);
}
