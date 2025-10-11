#include "historialwindow.h"
#include <QVBoxLayout>
#include <QHeaderView>

HistorialWindow::HistorialWindow(QWidget *parent)
    : QWidget(parent)
{
    // 1. Inicializar el administrador de historial (modelo de datos)
    m_historyManager = new HistoryManager(this);

    // 2. Inicializar el modelo de tabla, pasándole el manager.
    m_historyModel = new HistoryModel(m_historyManager, this);

    // 3. Inicializar y configurar la vista de tabla
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_historyModel);

    // Configuración de visualización
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::NoSelection); // No se permite selección
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->verticalHeader()->setVisible(false);

    // 🚨 MODIFICACIÓN: Mejorar el redimensionamiento de columnas para que los números se vean.
    // Usamos Stretch para Aplicaciones y ResizeToContents para el resto.

    // Columna 0: Aplicaciones - Toma el espacio restante
    m_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Las columnas numéricas se ajustan al contenido más ancho.
    m_tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // PID
    m_tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Memoria (MB)
    m_tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Tiempo CPU (s)
    m_tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Hora de Inicio
    m_tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); // Hora de Fin

    // 4. Configurar el layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_tableView);
    setLayout(mainLayout);
}
