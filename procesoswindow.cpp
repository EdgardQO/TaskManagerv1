#include "procesoswindow.h"
#include <QMenu>
#include <QMessageBox>
#include <QProcess> // Necesario para ejecutar comandos del sistema (kill)


ProcesosWindow::ProcesosWindow(QWidget *parent)
    : QWidget(parent)
{
    // 1. Inicializar componentes
    m_processModel = new ProcessTableModel(this);
    tableViewProcesos = new QTableView(this);

    // 2. Configurar la tabla
    tableViewProcesos->setModel(m_processModel);
    tableViewProcesos->setSortingEnabled(true);


    // 🚨 CONFIGURACIÓN CRUCIAL DE SELECCIÓN DE FILAS
    // 1. Establece el modo de selección: seleccionar filas enteras.
    tableViewProcesos->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 2. Establece el modo de selección: solo puedes seleccionar una fila a la vez.
    tableViewProcesos->setSelectionMode(QAbstractItemView::SingleSelection);
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
    // 🚨 MODIFICACIÓN: Conectar al nuevo slot de la propia ventana
    connect(m_timer, &QTimer::timeout, this, &ProcesosWindow::refreshProcessListAndMaintainSelection);
    m_timer->start(1000); // 1 segundo
}

// 🚨 NUEVA FUNCIÓN: Mantiene la selección antes y después de la actualización del modelo.
void ProcesosWindow::refreshProcessListAndMaintainSelection()
{
    int selectedPid = -1;

    // 1. Guardar el PID del proceso seleccionado (si hay alguno)
    QModelIndexList selectedRows = tableViewProcesos->selectionModel()->selectedRows();

    if (!selectedRows.isEmpty()) {
        QModelIndex index = selectedRows.first();
        // Obtener el PID (Columna 3) usando el índice en el modelo
        QVariant pidVariant = m_processModel->data(m_processModel->index(index.row(), 3), Qt::DisplayRole);
        selectedPid = pidVariant.toInt();
    }

    // 2. Actualizar la lista de procesos (esto llama a beginResetModel/endResetModel y pierde la selección)
    m_processModel->updateProcessList();

    // 3. Intentar re-seleccionar la fila usando el PID
    if (selectedPid != -1) {
        QModelIndex newIndex;
        // Buscar el PID en el modelo actualizado. Iterar sobre las filas.
        for (int row = 0; row < m_processModel->rowCount(); ++row) {
            QModelIndex pidIndex = m_processModel->index(row, 3); // Columna 3 es el PID
            if (m_processModel->data(pidIndex, Qt::DisplayRole).toInt() == selectedPid) {
                // Encontrado. Seleccionar la primera columna (columna 0) de esa fila
                newIndex = m_processModel->index(row, 0);
                break;
            }
        }

        if (newIndex.isValid()) {
            // Re-seleccionar la fila
            tableViewProcesos->selectRow(newIndex.row());
        }
    }
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
    QAction *terminateAction = contextMenu.addAction("Finalizar Tarea");
    connect(terminateAction, &QAction::triggered, this, &ProcesosWindow::terminateProcess);

    // 4. Mostrar el menú en la posición del mouse
    contextMenu.exec(tableViewProcesos->viewport()->mapToGlobal(pos));
}

void ProcesosWindow::terminateProcess()
{
    // 1. Obtener la fila seleccionada
    QModelIndexList selectedRows = tableViewProcesos->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ningún proceso.");
        return;
    }

    QModelIndex index = selectedRows.first();

    // 2. Obtener el PID y Nombre
    QVariant pidVariant = m_processModel->data(m_processModel->index(index.row(), 3), Qt::DisplayRole);
    int pid = pidVariant.toInt();
    QString processName = m_processModel->data(m_processModel->index(index.row(), 0), Qt::DisplayRole).toString();

    if (pid <= 0) {
        QMessageBox::critical(this, "Error", "No se pudo obtener el PID.");
        return;
    }

    // 🚨 SOLUCIÓN FINAL: Ejecutar el comando a través del shell (/bin/sh -c).
    // Esto es el equivalente de la terminal y es más robusto en entornos restringidos.
    QString commandToExecute = QString("/bin/kill -9 %1").arg(pid);

    // Usamos QProcess::execute para llamar a /bin/sh y pasarle el comando.
    int exitCode = QProcess::execute("/bin/sh", QStringList() << "-c" << commandToExecute);

    // 4. Notificación al usuario basada en el código de salida
    if (exitCode == 0) {
        QMessageBox::information(this, "Tarea Finalizada",
                                 QString("El proceso '%1' (PID %2) ha sido terminado.").arg(processName).arg(pid));
    } else {
        // Corrección del error 'QString::arg: Argument missing': solo usamos %1 y pasamos un argumento.
        QString errorMsg = "El comando de terminación falló. Código de salida: %1.";
        QMessageBox::critical(this, "Error de Terminación",
                              errorMsg.arg(exitCode));
    }

    // Forzar una actualización de la tabla
    m_processModel->updateProcessList();
}
