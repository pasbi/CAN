#ifndef ITEMOWNERCOMMANDDECORATOR_H
#define ITEMOWNERCOMMANDDECORATOR_H

template<typename T>
class ItemOwnerCommandDecorator
{
protected:
    ItemOwnerCommandDecorator(T* item) :
        m_item(item)
    {

    }

    ~ItemOwnerCommandDecorator()
    {
        if (m_ownsItem)
        {
            delete m_item;
            m_item = nullptr;
        }
    }

    void takeOwnershipOfItem()
    {
        m_ownsItem = true;
    }

    void releaseOwnershipOfItem()
    {
        m_ownsItem = false;
    }

    T* item() const
    {
        return m_item;
    }

private:
    T* m_item = nullptr;
    bool m_ownsItem = false;
};

#endif // ITEMOWNERCOMMANDDECORATOR_H
