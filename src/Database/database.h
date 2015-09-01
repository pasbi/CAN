#ifndef DATABASE_H
#define DATABASE_H

#include <QAbstractTableModel>
#include <QAbstractProxyModel>
#include <QJsonObject>
#include <QJsonDocument>

#include "persistentobject.h"
#include "Project/project.h"
#include "Database/databasemimedata.h"
#include "application.h"
#include "Commands/DatabaseCommands/databasenewitemcommand.h"

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
    void insertItem(T* item, int row = -1)
    {
        if (row < 0)
        {
            row = rowCount();
        }

        beginInsertRows(QModelIndex(), row, row);
        m_items.insert(row, item);
        endInsertRows();
    }

    void removeItem(T* item)
    {
        int row;
        if ( (row = m_items.indexOf(item)) < 0 )
        {
            WARNING << "Database does not contain the item " << item;
        }
        else
        {
            beginRemoveRows(QModelIndex(), row, row);
            m_items.removeAt(row);
            endRemoveRows();
        }
    }

    int rowOf(const T* item) const
    {
        int row = -1;
        for (int i = 0; i < m_items.length() && row < 0; ++i)
        {
            if (item == m_items[i])
            {
                row = i;
            }
        }
        assert(row > 0);
        return row;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        assert(!parent.isValid());
        return m_items.length();
    }

    virtual void reset()
    {
        beginResetModel();
        qDeleteAll( m_items );
        m_items.clear();
        endResetModel();
    }

    virtual QString fileNameBase() const = 0;

    bool restoreFromJsonObject(const QJsonObject &object)
    {
        beginResetModel();
        bool success = PersistentObject::restoreFromJsonObject(object);

        m_items.clear();
        QStringList filenames = QDir( project()->path() ).entryList( QStringList() << QString("%1*").arg(fileNameBase()) );
        filenames.removeOne(QString("%1Database").arg(fileNameBase()));

        for (const QString& filename : filenames)
        {
            T* s = new T(this);
            success &= s->loadFrom( project()->makeAbsolute(filename ));
            m_items << s;
        }

        endResetModel();
        return success;
    }

    QList<File> getFiles() const
    {
        QList<File> files;
        files << File( QString("%1Database").arg(fileNameBase()), QJsonDocument(toJsonObject()).toJson() );

        for (int i = 0; i < m_items.size(); ++i)
        {
            files << File( QString("%1%2").arg(fileNameBase()).arg( m_items[i]->randomID() ),
                           QJsonDocument(m_items[i]->toJsonObject()).toJson() );
        }

        return files;
    }

    Qt::DropActions supportedDropActions() const
    {
        return Qt::IgnoreAction;
    }

    QMimeData* mimeData(const QModelIndexList &indexes) const
    {
        DatabaseMimeData<T>* mime = new DatabaseMimeData<T>();
        for (const QModelIndex& index : indexes)
        {
            if (index.column() != 0)
            {
                // we want only one index per row.
                continue;
            }
            mime->append(itemAtIndex(index), index.row());
        }
        return mime;
    }

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
    {
        if (action == Qt::IgnoreAction)
        {
            return true;
        }

        if (parent.isValid())
        {
            // no support for drag onto items
            return false;
        }

        // drag'n'drop only whole rows, so column is arbitrary
        Q_UNUSED(column);
        // since
        Q_UNUSED(row);

        // check if data is in right format
        const DatabaseMimeData<T>* itemData = DatabaseMimeData<T>::cast(data);
        if (!itemData)
        {
            return false;
        }

        // copy paste
        if (action == Qt::CopyAction)
        {
            app().beginMacro( tr("Paste") );
            int i = 0;
            typedef typename DatabaseMimeData<T>::IndexedItem IndexedItem;
            for (IndexedItem item : itemData->indexedItems())
            {
                app().pushCommand( new DatabaseNewItemCommand<T>( this, item.item->copy(), row + i ) );
                i++;
            }
            app().endMacro();
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }

    virtual bool columnIsVisible(int)
    {
        return true;
    }

private:
    Project* m_project;
protected:
    QList<T*> m_items;
};

#endif // DATABASE_H
