// recursos.cpp
#include "recursos.h"
#include "cpuwidget.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QScrollArea>

Recursos::Recursos(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    scrollAreaWidget = new QWidget();
    scrollLayout = new QVBoxLayout(scrollAreaWidget);
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollLayout->setContentsMargins(10, 10, 10, 10);
    scrollLayout->setSpacing(5);

    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addWidget(scrollArea);

    // --- Creación de las Secciones ---

    // 1. Sección de CPU
    cpuWidget = new CPUWidget(this);
    scrollLayout->addWidget(createCollapsibleSection("CPU", cpuWidget, true)); // ✅ Inicia desplegado

    // 2. Marcadores de posición
    QLabel *memoriaWidget = new QLabel("Gráfica de Memoria irá aquí.", this);
    scrollLayout->addWidget(createCollapsibleSection("Memoria e Intercambio", memoriaWidget));

    QLabel *redWidget = new QLabel("Gráfica de Red irá aquí.", this);
    scrollLayout->addWidget(createCollapsibleSection("Red", redWidget));

    QLabel *discoWidget = new QLabel("Gráfica de Disco irá aquí.", this);
    scrollLayout->addWidget(createCollapsibleSection("Disco", discoWidget));
}

// ✅ Añadimos un parámetro 'startChecked' para controlar el estado inicial.
QWidget* Recursos::createCollapsibleSection(const QString &title, QWidget *contentWidget, bool startChecked)
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QToolButton *toggleButton = new QToolButton(this);
    toggleButton->setText(title);
    toggleButton->setCheckable(true);
    toggleButton->setChecked(startChecked);
    toggleButton->setStyleSheet(
        "QToolButton { border: none; font-weight: bold; text-align: left; padding: 5px;}"
        "QToolButton:checked { background-color: #e0e0e0; }"
        );
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(startChecked ? Qt::DownArrow : Qt::RightArrow);

    connect(toggleButton, &QToolButton::toggled, this, [=](bool checked) {
        toggleButton->setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
        contentWidget->setVisible(checked);
    });

    contentWidget->setVisible(startChecked);

    layout->addWidget(toggleButton);
    layout->addWidget(contentWidget);

    return container;
}
