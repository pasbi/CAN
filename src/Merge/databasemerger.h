#ifndef DATABASEMERGER_H
#define DATABASEMERGER_H

#include "databasemergerbase.h"
#include "Database/database.h"
#include "mergeitem.h"

template<class T>
class DatabaseMerger : public DatabaseMergerBase
{
public:
    DatabaseMerger(Database<T>* masterDatabase, Database<T>* slaveDatabase);
    void performMerge(NewPointerTable &updatePointers, QList<const void *> &undeletableItems);
    void init(const QList<T*>& masterItems, const QList<T*>& slaveItems);

    Database<T>* masterDatabase() const;
    Database<T>* slaveDatabase() const;

protected:
    DatabaseMergerBase* createChildDatabaseMerger(DatabaseItemBase *masterItem, DatabaseItemBase *slaveItem) const;

private:
    Database<T>* m_masterDatabase;
    Database<T>* m_slaveDatabase;

    template<class S> static QList<T> convertList(const QList<S>& list);

    void performMergePrivate(NewPointerTable &updatePointers, QList<const void *> &undeletableItems);

};



#endif // DATABASEMERGER_H
