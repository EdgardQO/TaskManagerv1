#ifndef STARTUPMODEL_H
#define STARTUPMODEL_H

#include <QAbstractTableModel>
#include <QList>

struct StartupEntry {
    QString name;
    QString editor;
    QString command; // Ruta al ejecutable o comando
    QString status; // "Habilitado" o "Deshabilitado"
    QString impact; // "Alto", "Medio", "Bajo"
};

class StartupModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StartupModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QList<StartupEntry> m_startupList;
    void fetchStartupEntries();
};

#endif // STARTUPMODEL_H
