#include "rendimientowindow.h"
#include <QVBoxLayout>

RendimientoWindow::RendimientoWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // 1. CPU Widget
    CPUWidget *cpuWidget = new CPUWidget(this);
    mainLayout->addWidget(cpuWidget);

    // 2. Recursos Widget (Memoria, Swap, etc.)
    Recursos *recursosWidget = new Recursos(this);
    mainLayout->addWidget(recursosWidget);

    // El stretch asegura que los widgets se peguen a la parte superior
    mainLayout->addStretch();
}
