#ifndef MERGEITEM_H
#define MERGEITEM_H

#include <Qt>

class DatabaseItemBase;

class MergeItem
{
public:
    enum Type { Add, Remove, Modify };
    enum Action { AddAction, RemoveAction, ModifyAction };
    MergeItem(DatabaseItemBase* master, DatabaseItemBase* slave);
    MergeItem(DatabaseItemBase* item, Type type, Action action);

    DatabaseItemBase* master() const;
    DatabaseItemBase* slave() const;
    QString label() const;
    Type type() const;
    Action action() const;
    void setAction(Action action);

private:
    Type m_type;
    Action m_action;
    DatabaseItemBase* m_master;
    DatabaseItemBase* m_slave;


    //disable copy
    MergeItem(const MergeItem& other) = delete;
    MergeItem& operator=(const MergeItem& other) = delete;
};

#endif // MERGEITEM_H
