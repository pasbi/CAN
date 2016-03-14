#ifndef MERGEITEM_H
#define MERGEITEM_H

#include "map.h"
#include "global.h"

class DatabaseMergerBase;

class DatabaseItemBase;

class MergeItem
{
public:
    enum Origin { Master = 0x1, Slave = 0x2, Both = 0x3 };
    enum Action { AddAction, RemoveAction, ModifyAction, NoAction };
    enum Decision { UseMaster, UseSlave, /*UseUserValue*/ };
    MergeItem(DatabaseItemBase* master, DatabaseItemBase* slave, Action action);
    MergeItem(DatabaseItemBase* item, Origin origin, Action action);
    ~MergeItem();

    DatabaseItemBase* master() const;
    DatabaseItemBase* slave() const;
    QString label() const;
    Origin origin() const;
    Action action() const;
    void setAction(Action action);
    bool canJoin(const MergeItem* other) const;

    bool inherits(const QStringList& classnames) const;


    struct ModifyDetail
    {
        ModifyDetail(const QString& key, Decision initialDecision) :
            m_key(key),
            m_decision(initialDecision)
        {

        }

        QString key() const { return m_key; }
        Decision decision() const { return m_decision; }
        void setDecision(Decision decision) { m_decision = decision; }

    private:
        QString m_key;
        Decision m_decision;
    };

    QList<ModifyDetail> modifyDetails() const;
    ModifyDetail modifyDetail(const QString& key) const;

    void updateModifyDetails(const QList<ModifyDetail> modifyDetails);

    /**
     * @brief performModification modify the master item according to modifyDetails.
     */
    void performModification() const;

    void setChildMerger(DatabaseMergerBase* childMerger);

    // ownership is transfered to caller
    DatabaseMergerBase* childMerger() const;


private:
    Origin m_origin;
    Action m_action;
    DatabaseItemBase* m_master;
    DatabaseItemBase* m_slave;

    // if type is Modify, use this map.
    QList<ModifyDetail> m_modifyDetails;
    void setupModifyDetails();

    DatabaseMergerBase* m_childMerger;


    //disable copy
    MergeItem(const MergeItem& other) = delete;
    MergeItem& operator=(const MergeItem& other) = delete;

public:
    static bool sortMasterSlaveItem(MergeItem*& masterItem, MergeItem*& slaveItem);
    static bool sortMasterSlaveItem(const MergeItem*& masterItem, const MergeItem*& slaveItem);


};

DECLARE_ENUM_STREAM_OPERATORS(MergeItem::Action)
DECLARE_ENUM_STREAM_OPERATORS(MergeItem::Decision)

Q_DECLARE_METATYPE(MergeItem::Action)
Q_DECLARE_METATYPE(MergeItem::Decision)
REGISTER_META_TYPE_STREAM_OPERATORS(MergeItem::Action, MergeItemAction)
REGISTER_META_TYPE_STREAM_OPERATORS(MergeItem::Decision, MergeItemDecision)


#endif // MERGEITEM_H
