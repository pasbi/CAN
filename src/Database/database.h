#ifndef DATABASE_H
#define DATABASE_H

#include "databasebase.h"

template<typename T>
class Database : public DatabaseBase
{
private:
    Database(Project* project);
    virtual ~Database();

public:
    QList<T*> items() const;

    /**
     * @brief item returns the item that has pointer ptr.
     * @note return nullptr if database does not has any such item.
     * @note equivalent code: T* t = static_cast<T*>(const_cast<void*>(ptr)); return items().contains(t)?t:nullptr;
     */
    T* item(const void* ptr) const;
    void insertItem(T* item, int row = -1);
    void removeItem(T* item);
    int rowOf(const T* item) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual void reset();
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    T* itemAtIndex(const QModelIndex& index) const;
    void notifiyDataChange(const T *item);
    T* retrieveItem(qint32 id) const;
    int identifyItem(const T* item) const;
    QModelIndex indexOf(const T* item, int column = 0);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QList<T*> m_items;


protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

    // moving rows is a quite complicated task which is done completely in the comand.
    // thus it needs low level access to this.
    template<typename S> friend class DatabaseMoveRowsCommand;

    // Each template instanciation should be subclassed by _exactly_one_ class.
    // with this assumption, we can  static_cast  e.g. a  Database<Song>  safely to a  SongDatabase.
    // for this reason, constructors and destructors are private. All classes which are allowed to derive this template are listed below.
    friend class SongDatabase;          //Database<Song>
    friend class EventDatabase;         //Database<Event>
    friend class Setlist;               //Database<SetlistItem>
    friend class AttachmentDatabase;    //Database<Attachment>

};

#endif // DATABASE_H
