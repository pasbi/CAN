#ifndef COMBINEITEMS_H
#define COMBINEITEMS_H

#include <QVariant>
#include "map.h"
#include "Database/databaseitem.h"

template<class T> class Database;

struct MergeAtom
{
public:
    enum Decision { None, Master, Slave };
    MergeAtom(const DatabaseItemBase* masterItem, const DatabaseItemBase* slaveItem, const QString& key) :
        m_decision(None),
        m_masterValue(masterItem->attribute(key)),
        m_slaveValue(slaveItem->attribute(key)),
        m_masterDisplay(masterItem->attributeDisplay(key)),
        m_slaveDisplay(slaveItem->attributeDisplay(key))
    {
    }

    void setDecision(Decision decision)
    {
        Q_ASSERT(decision != None);
        m_decision = decision;
    }

    QVariant masterValue() const { return m_masterValue; }
    QVariant slaveValue() const { return m_slaveValue; }
    QString masterDisplay() const { return m_masterDisplay; }
    QString slaveDisplay() const { return m_slaveDisplay; }
    QVariant value() const
    {
        switch (m_decision)
        {
        case Master:
            return masterValue();
        case Slave:
            return slaveValue();
        case None:
        default:
            Q_UNREACHABLE();
            return QVariant();
        }
    }

private:
    Decision m_decision;
    QVariant m_masterValue;
    QVariant m_slaveValue;
    QString m_masterDisplay;
    QString m_slaveDisplay;

};

template<class T>
class CombineItems
{
protected:
    CombineItems(const T* master, const T* slave) :
        m_master(master),
        m_slave(slave)
    {
    }

    void addItem(const QString& key, const MergeAtom& atom)
    {
        m_mergeAtoms.append(key, atom);
    }

protected:
    const T* master() const { return m_master; }
    const T* slave() const { return m_slave; }

private:
    const T* m_master;
    const T* m_slave;
    SortedMap<QString, MergeAtom> m_mergeAtoms;


};

#endif // COMBINEITEMS_H
