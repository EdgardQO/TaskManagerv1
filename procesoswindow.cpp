#include "procesoswindow.h"
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QScrollBar> // Necesario para el manejo del scroll
#include <QSet> // Útil para almacenar PIDs de forma eficiente

ProcesosWindow::ProcesosWindow(QWidget *parent)
    : QWidget(parent)
{
    // 1. Inicializar componentes
    m_processModel = new ProcessTableModel(this);
    tableViewProcesos = new QTableView(this);

    // 2. Configurar la tabla
    tableViewProcesos->setModel(m_processModel);
    tableViewProcesos->setSortingEnabled(true);


    // 🚨 MODIFICACIÓN 1: Permitir Selección Múltiple
    // 1. Establece el modo de selección: seleccionar filas enteras.
    tableViewProcesos->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 2. CAMBIO: Permite selección múltiple (ExtendedSelection: Ctrl/Shift para seleccionar varios)
    tableViewProcesos->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 3. Habilita el menú contextual (clic derecho)
    tableViewProcesos->setContextMenuPolicy(Qt::CustomContextMenu);
    // ----------------------------------------------------

    // Ajuste de columnas
    tableViewProcesos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableViewProcesos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableViewProcesos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);


    //orden --> VERIFICAR FUNCIONALIDAD PORQUE ES UNA BASURA ACTUALMENTE
    tableViewProcesos->sortByColumn(0,Qt::AscendingOrder);

    // 🚨 CONECTAR LA SEÑAL DEL MENÚ CONTEXTUAL
    connect(tableViewProcesos, &QTableView::customContextMenuRequested,
            this, &ProcesosWindow::on_tableViewProcesos_customContextMenuRequested);


    // 3. Configurar el layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableViewProcesos);
    setLayout(mainLayout);

    // 4. Configurar e iniciar el temporizador
    m_timer = new QTimer(this);
    // Conectar al slot que mantiene la selección y el scroll
    connect(m_timer, &QTimer::timeout, this, &ProcesosWindow::refreshProcessListAndMaintainSelection);
    m_timer->start(1000); // 1 segundo
}

// 🚨 MODIFICACIÓN 2: Mantiene la selección Y LA POSICIÓN DEL SCROLL
void ProcesosWindow::refreshProcessListAndMaintainSelection()
{
    // 1. Guardar la posición del scroll vertical ANTES de la actualización
    int scrollValue = tableViewProcesos->verticalScrollBar()->value();

    // 2. Guardar los PIDs de todos los procesos seleccionados
    QSet<int> selectedPids;
    QModelIndexList selectedRows = tableViewProcesos->selectionModel()->selectedRows();

    for (const QModelIndex &index : selectedRows) {
        // Obtener el PID (Columna 3)
        // Usamos la columna 3 (PID) para identificar de forma única
        QVariant pidVariant = m_processModel->data(m_processModel->index(index.row(), 3), Qt::DisplayRole);
        selectedPids.insert(pidVariant.toInt());
    }

    // 3. Actualizar la lista de procesos (esto llama a beginResetModel/endResetModel y pierde selección)
    m_processModel->updateProcessList();

    // 4. Intentar re-seleccionar las filas usando los PIDs guardados
    if (!selectedPids.isEmpty()) {
        QItemSelection newSelection;

        for (int row = 0; row < m_processModel->rowCount(); ++row) {
            QModelIndex pidIndex = m_processModel->index(row, 3); // Columna 3 es el PID

            if (selectedPids.contains(m_processModel->data(pidIndex, Qt::DisplayRole).toInt())) {
                // Encontrado. Crear una selección para toda la fila
                QModelIndex start = m_processModel->index(row, 0);
                QModelIndex end = m_processModel->index(row, m_processModel->columnCount() - 1);
                newSelection.select(start, end);
            }
        }

        // 🚨 CAMBIO CLAVE: Aplicar la selección de una vez usando QItemSelectionModel::select.
        // Se usa ClearAndSelect para reemplazar la selección anterior (perdida) y Rows para asegurar la fila completa.
        // Al NO usar el método 'selectRow(row)', evitamos que se intente establecer un índice 'current'
        // que es lo que usualmente fuerza el autoscroll.
        tableViewProcesos->selectionModel()->select(newSelection, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }

    // 5. Restaurar la posición del scroll vertical
    // Esto se hace al final para anular cualquier intento de autoscroll de la vista.
    tableViewProcesos->verticalScrollBar()->setValue(scrollValue);
}

void ProcesosWindow::on_tableViewProcesos_customContextMenuRequested(const QPoint &pos)
{
    // 1. Obtener el índice de la fila donde se hizo clic
    QModelIndex index = tableViewProcesos->indexAt(pos);

    // Si el clic no fue sobre una fila válida, no mostramos el menú
    if (!index.isValid())
        return;

    // 2. Crear el menú
    QMenu contextMenu(this);

    // 3. Crear la acción "Finalizar Tarea" y conectarla
    QAction *terminateAction = contextMenu.addAction("Finalizar Tarea(s)");
    connect(terminateAction, &QAction::triggered, this, &ProcesosWindow::terminateProcess);

    // 4. Mostrar el menú en la posición del mouse
    contextMenu.exec(tableViewProcesos->viewport()->mapToGlobal(pos));
}

// 🚨 MODIFICACIÓN 3: Termina múltiples procesos con una sola autenticación
void ProcesosWindow::terminateProcess()
{
    // 1. Obtener todas las filas seleccionadas
    QModelIndexList selectedRows = tableViewProcesos->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ningún proceso.");
        return;
    }

    // 2. Recolectar todos los PIDs seleccionados y los nombres de los procesos
    QStringList pidsToKill;
    QStringList processNames;

    for (const QModelIndex &index : selectedRows) {
        // Obtener el PID (Columna 3) y el nombre (Columna 0)
        int pid = m_processModel->data(m_processModel->index(index.row(), 3), Qt::DisplayRole).toInt();
        QString name = m_processModel->data(m_processModel->index(index.row(), 0), Qt::DisplayRole).toString();

        if (pid > 0) {
            pidsToKill << QString::number(pid);
            processNames << name;
        }
    }

    if (pidsToKill.isEmpty()) {
        QMessageBox::critical(this, "Error", "No se pudieron obtener PIDs válidos para terminar.");
        return;
    }

    // 3. Construir la lista de argumentos para pkexec
    // Sintaxis: pkexec kill -9 PID1 PID2 PID3 ...
    QStringList arguments;
    arguments << "kill" << "-9";
    arguments << pidsToKill; // Añade todos los PIDs a la lista de argumentos

    // 4. Ejecutar el comando a través de pkexec (solicita la clave una sola vez)
    QProcess process;
    process.start("pkexec", arguments);
    process.waitForFinished();

    int exitCode = process.exitCode();
    QString standardError = process.readAllStandardError();
    QString processSummary = (pidsToKill.size() == 1) ? processNames.first() :
                                 QString("%1 procesos (ej: %2...)").arg(pidsToKill.size()).arg(processNames.first());

    // 5. Notificación al usuario
    if (exitCode == 0) {
        QMessageBox::information(this, "Tareas Finalizadas",
                                 QString("Se intentó terminar %1 proceso(s) (Ej: %2). La operación fue aceptada por el sistema.").arg(pidsToKill.size()).arg(processSummary));
    } else {
        QString errorMsg = QString("El comando de terminación falló para %1 proceso(s).\n"
                                   "Código de salida: %2.\n"
                                   "Error del sistema: %3").arg(pidsToKill.size()).arg(exitCode).arg(standardError.trimmed());
        QMessageBox::critical(this, "Error de Terminación", errorMsg);
    }

    // Forzar una actualización de la tabla para ver los resultados
    m_processModel->updateProcessList();
}
