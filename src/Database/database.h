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
#include "Database/EventDatabase/setlistitem.h"

#include "util.h"
#include "itemhastypenameinterface.h"

class DatabaseBase : public QAbstractTableModel, public PersistentObject/*, public ItemHasTypenameInterface*/
{
    Q_OBJECT
protected:
    DatabaseBase(Project *project, QObject *parent = nullptr);
    virtual ~DatabaseBase();
public:
    Project* project() const;

    Qt::DropActions supportedDropActions() const;

private:
    Project* m_project;
};


template<typename T>
class Database : public DatabaseBase
{
private:
    Database(Project* project) :
        DatabaseBase(project)
    {

    }

    virtual ~Database()
    {

    }

public:
    QList<T*> items() const
    {
        return m_items;
    }

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
        int row = m_items.indexOf(item);
        assert(row >= 0);
        beginRemoveRows(QModelIndex(), row, row);
        m_items.removeAt(row);
        endRemoveRows();
    }

    int rowOf(const T* item) const
    {
        return indexOfConstInList( m_items, item );
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
            app().beginMacro( DatabaseBase::tr("Paste") );
            int i = 0;
            typedef typename DatabaseMimeData<T>::IndexedItem IndexedItem;
            for (IndexedItem item : itemData->indexedItems())
            {
                app().pushCommand( new DatabaseNewItemCommand<T>( this, item.item->copy(this), row + i ) );
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

    T* itemAtIndex(const QModelIndex& index) const
    {
        assert( index.model() == this );

        if (index.isValid())
        {
            return m_items[index.row()];
        }
        else
        {
            return nullptr;
        }

    }

    void notifiyDataChange(const T *item)
    {
        int row = rowOf(item);
        emit dataChanged( index(row, 0), index(row, columnCount() - 1) );
    }

    void serialize(QDataStream &out) const
    {
        out << (qint32) m_items.length();
        for (T* item : m_items)
        {
            out << item;
        }
    }

    void deserialize(QDataStream &in)
    {
        reset();
        qint32 n;
        in >> n;
        beginResetModel();
        for (int i = 0; i < n; ++i)
        {
            T* item = new T(this);
            in >> item;
            m_items << item;
        }
        endResetModel();
    }

    T* retrieveItem(qint32 id) const
    {
        return m_items[id];
    }

    int identifyItem(const T* item) const
    {
        return m_items.indexOf(const_cast<T*>(item));
    }

    QString itemTypeName() const
    {
        return T::TYPE_NAME;
    }


protected:
    QList<T*> m_items;

    // moving rows is a quite complicated task which is done completely in the comand.
    // thus it needs low level access to this.
    template<typename S> friend class DatabaseMoveRowsCommand;

    // Each template instanciation should be subclassed by _exactly_one_ class.
    // with this assumption, we can  static_cast  e.g. a  Database<Song>  safely to a  SongDatabase.
    // for this reason, constructors and destructors are private. All classes which are allowed to derive this template are listed below.
    friend class SongDatabase;
    friend class EventDatabase;
    friend class Setlist;
};

#endif // DATABASE_H
