// recursos.h
#ifndef RECURSOS_H
#define RECURSOS_H

#include <QWidget>

// Declaraciones adelantadas
class QVBoxLayout;
class QScrollArea;
class CPUWidget;

class Recursos : public QWidget
{
    Q_OBJECT

public:
    explicit Recursos(QWidget *parent = nullptr);
    ~Recursos() override = default;

private:
    // ✅ CORRECCIÓN: Añadimos el tercer parámetro con un valor por defecto.
    QWidget* createCollapsibleSection(const QString &title, QWidget *contentWidget, bool startChecked = false);

    // Layout principal
    QVBoxLayout *mainLayout;

    // Contenedor con scroll para que quepan todas las gráficas.
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidget;
    QVBoxLayout *scrollLayout;

    // Nuestros widgets de gráficas
    CPUWidget *cpuWidget;
};

#endif // RECURSOS_H
