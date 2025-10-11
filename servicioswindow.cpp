#include "servicioswindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QProcess>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <QAbstractItemView>

ServiciosWindow::ServiciosWindow(QWidget *parent) : QWidget(parent) {
    // Usamos 'this' (QWidget) como contenedor principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5); // Márgenes para el contenido

    // ---- ENCABEZADO ---- (Copiado de la lógica de ServiceManager)
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *title = new QLabel("<h2>Servicios</h2>");
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(title);

    headerLayout->addStretch();

    btnStart = new QPushButton("🟢 Iniciar");
    btnStop = new QPushButton("🔴 Detener");
    btnRestart = new QPushButton("🔄 Reiniciar");

    btnStart->setEnabled(false);
    btnStop->setEnabled(false);
    btnRestart->setEnabled(false);

    headerLayout->addWidget(btnStart);
    headerLayout->addWidget(btnStop);
    headerLayout->addWidget(btnRestart);

    mainLayout->addLayout(headerLayout);
    // ---------------------

    table = new QTableWidget;
    table->setColumnCount(3);
    QStringList headers = {"Servicio", "Estado", "Descripción"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(table);

    statusLabel = new QLabel("Listo.");
    mainLayout->addWidget(statusLabel);

    cargarServicios();

    // Menú contextual
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested,
            this, &ServiciosWindow::mostrarMenu);

    // Selección de fila
    connect(table, &QTableWidget::itemSelectionChanged, this, &ServiciosWindow::actualizarBotones);

    // Botones superiores
    connect(btnStart, &QPushButton::clicked, this, [=]() { ejecutarAccion("start"); });
    connect(btnStop, &QPushButton::clicked, this, [=]() { ejecutarAccion("stop"); });
    connect(btnRestart, &QPushButton::clicked, this, [=]() { ejecutarAccion("restart"); });

    // Actualización periódica
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ServiciosWindow::cargarServicios);
    timer->start(20000);
}

void ServiciosWindow::cargarServicios() {
    table->setRowCount(0);

    QProcess process;
    // Comando 'systemctl' de ServiciosRepositorio
    process.start("bash", QStringList() << "-c"
                                        << "systemctl list-units --type=service --all --no-pager --no-legend");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    QStringList lines = output.split("\n");
    for (const QString &line : lines) {
        if (line.trimmed().isEmpty()) continue;
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() < 5) continue;
        QString service = parts[0];
        QString active = parts[2];
        QString desc = parts.mid(4).join(" ");
        QString estado = (active == "active") ? "En ejecución" : "Detenido";


        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(service));

        QTableWidgetItem *estadoItem = new QTableWidgetItem(estado);
        estadoItem->setForeground(estado == "En ejecución" ? Qt::green : Qt::red);
        table->setItem(row, 1, estadoItem);

        table->setItem(row, 2, new QTableWidgetItem(desc));
    }
    statusLabel->setText("Servicios cargados: " + QString::number(table->rowCount()));
    actualizarBotones();
}

void ServiciosWindow::mostrarMenu(const QPoint &pos) {
    int row = table->currentRow();
    if (row < 0) return;

    QString servicio = table->item(row, 0)->text();
    QString estado = table->item(row, 1)->text();

    QMenu menu;
    if (estado == "Detenido") {
        QAction *start = menu.addAction("Iniciar");
        connect(start, &QAction::triggered, [=]() { ejecutarAccion("start", servicio); });
    } else {
        QAction *stop = menu.addAction("Detener");
        QAction *restart = menu.addAction("Reiniciar");
        connect(stop, &QAction::triggered, [=]() { ejecutarAccion("stop", servicio); });
        connect(restart, &QAction::triggered, [=]() { ejecutarAccion("restart", servicio); });
    }
    menu.exec(table->viewport()->mapToGlobal(pos));
}

void ServiciosWindow::ejecutarAccion(const QString &accion, QString servicio) {
    if (servicio.isEmpty()) {
        int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(this, "Sin selección", "Seleccione un servicio primero.");
            return;
        }
        servicio = table->item(row, 0)->text();
    }

    QProcess process;
    // Uso de 'pkexec' para permisos, copiado de ServiciosRepositorio
    process.start("pkexec", QStringList() << "systemctl" << accion << servicio);
    process.waitForFinished();
    cargarServicios();
}

void ServiciosWindow::actualizarBotones() {
    int row = table->currentRow();
    if (row < 0) {
        btnStart->setEnabled(false);
        btnStop->setEnabled(false);
        btnRestart->setEnabled(false);
        return;
    }

    QString estado = table->item(row, 1)->text();
    if (estado == "En ejecución") {
        btnStart->setEnabled(false);
        btnStop->setEnabled(true);
        btnRestart->setEnabled(true);
    } else {
        btnStart->setEnabled(true);
        btnStop->setEnabled(false);
        btnRestart->setEnabled(false);
    }
}
