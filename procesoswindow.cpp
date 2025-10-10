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

    // El modo de selección es SingleSelection, así que solo tomamos la primera fila
    QModelIndex index = selectedRows.first();

    // 2. Obtener el PID del proceso seleccionado (PID es la Columna 3)
    QVariant pidVariant = m_processModel->data(m_processModel->index(index.row(), 3), Qt::DisplayRole);
    int pid = pidVariant.toInt();
    QString processName = m_processModel->data(m_processModel->index(index.row(), 0), Qt::DisplayRole).toString();

    if (pid <= 0) {
        QMessageBox::critical(this, "Error", "No se pudo obtener el PID.");
        return;
    }

    // 🚨 MODIFICACIÓN: Usar el comando 'pkexec' para obtener privilegios SOLAMENTE para 'kill'.
    // pkexec es el método moderno y más seguro que gksu o el uso directo de 'sudo' en GUI.

    QString command;
    // La forma más robusta es a través de pkexec, que pide la contraseña gráficamente.
    // pkexec es preferido en muchos entornos Linux modernos.
    if (QProcess::execute("which pkexec") == 0) {
        command = QString("pkexec kill -9 %1").arg(pid);
    } else {
        // Opción de fallback: usar solo sudo. Esto podría no funcionar si no estás en una terminal.
        command = QString("sudo kill -9 %1").arg(pid);
    }

    // Ejecutar el comando. Si usamos pkexec, la ventana de contraseña aparecerá.
    int exitCode = QProcess::execute(command);

    // 4. Notificación al usuario basada en el código de salida
    if (exitCode == 0) {
        QMessageBox::information(this, "Tarea Finalizada",
                                 QString("El proceso '%1' (PID %2) ha sido terminado.").arg(processName).arg(pid));
    } else {
        // El comando kill falló.
        QString errorMsg;
        if (exitCode == 256 || exitCode == 1) {
            errorMsg = "Permiso denegado. Se necesita tu contraseña para terminar procesos críticos.";
        } else {
            errorMsg = "Error al ejecutar 'kill'. Código de salida: %2.";
        }

        QMessageBox::critical(this, "Error de Terminación",
                              errorMsg.arg(pid).arg(exitCode));
    }

    // Forzar una actualización de la tabla para que el proceso desaparecido se quite
    m_processModel->updateProcessList();
}
