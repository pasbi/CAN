#ifndef DATABASE_H
#define DATABASE_H

#include "persistentobject.h"
#include <QAbstractTableModel>
#include <QAbstractProxyModel>

class Project;
template<typename T>
class Database : public PersistentObject, public QAbstractTableModel
{
public:
    Database(Project* project) :
        m_project(project)
    {

    }

    Project* project() const
    {
        return m_project;
    }

    virtual QList<File> getFiles() const = 0;

    T* itemAtIndex(QModelIndex index) const
    {
        if (!index.isValid())
        {
            return nullptr;
        }
        else
        {
            // index may be pointing to an underlying proxy model
            const QAbstractItemModel* model = index.model();
            const QAbstractProxyModel* proxyModel;

            while (model && (proxyModel = qobject_cast<const QAbstractProxyModel*>(model)))
            {
                model = proxyModel->sourceModel();
                index = proxyModel->mapToSource(index);
            }

            if (!model || !index.isValid())
            {
                return nullptr;
            }

            if (model != this)
            {
                qWarning() << "Trying to retrieve item from wrong model " << model;
                return nullptr;
            }

            return m_items[index.row()];
        }
    }

public:
    QList<T*> items() const { return m_items; }
private:
    Project* m_project;
protected:
    QList<T*> m_items;
};

#endif // DATABASE_H
