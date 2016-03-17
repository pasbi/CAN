#ifndef ITEMOWNERCOMMANDDECORATOR_H
#define ITEMOWNERCOMMANDDECORATOR_H

template<typename T>
class ItemOwnerCommandDecorator
{
public:
    ItemOwnerCommandDecorator(T* item) :
        m_item(item)
    {

    }

    ~ItemOwnerCommandDecorator()
    {
        // Q_ASSERT that the type of T is known, thus the correct destructor is called
        static_assert( sizeof(T) > 0, "Invalid use of incomplete type" );
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
