#ifndef DATABASEREMOVEITEMCOMMAND_H
#define DATABASEREMOVEITEMCOMMAND_H

#include "Commands/itemownercommanddecorator.h"
#include "Commands/modelcommand.h"

template<typename T> class Database;
template<typename T>
class DatabaseRemoveItemCommand : public ModelCommand<Database<T>>, private ItemOwnerCommandDecorator<T>
{
public:
    DatabaseRemoveItemCommand( Database<T>* database, T* item ) :
        ModelCommand<Database<T>>( database ),
        ItemOwnerCommandDecorator<T>(item),
        m_index(database->rowOf(item))
    {
        assert( item );
    }

    ~DatabaseRemoveItemCommand()
    {
    }

    void undo()
    {
        this->releaseOwnershipOfItem();
        this->model()->insertItem( this->item(), m_index );
    }

    void redo()
    {
        this->takeOwnershipOfItem();
        this->model()->removeItem( this->item() );
    }

private:
    const int m_index;
};

#endif // DATABASEREMOVEITEMCOMMAND_H
