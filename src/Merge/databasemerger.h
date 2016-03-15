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
    void init(QList<T*> masterItems, QList<T*> slaveItems);

    Database<T>* masterDatabase() const;
    Database<T>* slaveDatabase() const;

protected:
    // 1: Equal
    // greater similarThreshold but smaller 0: Similar
    // smaller similarThreshold: Unequal
    // shall be >= 0, <= 1.
    DatabaseMergerBase::CompareResult compare(const T *a, const T *b) const;
    DatabaseMergerBase* createChildDatabaseMerger(DatabaseItemBase *masterItem, DatabaseItemBase *slaveItem) const;

private:
    Database<T>* m_masterDatabase;
    Database<T>* m_slaveDatabase;

    template<class S> static QList<T> convertList(const QList<S>& list);

};



#endif // DATABASEMERGER_H
