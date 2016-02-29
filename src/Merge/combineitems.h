#ifndef COMBINEITEMS_H
#define COMBINEITEMS_H

#include <Qt>

template<class T> class Database;

template<class T>
class CombineItems
{
protected:
    CombineItems(const T* master, const T* slave) :
        m_master(master),
        m_slave(slave)
    {
        m_combination = new T(m_master->database());
    }
    virtual void assembleCombination(T* result) = 0;

public:
    T* combination() const
    {
        return m_combination;
    }

protected:
    const T* master() const { return m_master; }
    const T* slave() const { return m_slave; }

private:
    const T* m_master;
    const T* m_slave;
    T* m_combination;
};

#endif // COMBINEITEMS_H
