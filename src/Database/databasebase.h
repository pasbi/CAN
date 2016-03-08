#ifndef DATABASEBASE_H
#define DATABASEBASE_H

#include <QAbstractTableModel>
#include <QAbstractProxyModel>
#include <QJsonObject>
#include <QJsonDocument>

#include "persistentobject.h"

class Project;

class DatabaseBase : public QAbstractTableModel, public PersistentObject
{
    Q_OBJECT
protected:
    DatabaseBase(Project *project, QObject *parent = nullptr);
    virtual ~DatabaseBase();
    void reset();

public:
    Project* project() const;

    Qt::DropActions supportedDropActions() const;
    virtual QString itemName(int n) const = 0;

private:
    Project* m_project;

signals:
    void reseted();
};

#endif // DATABASEBASE_H
