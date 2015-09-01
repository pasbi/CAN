#ifndef DATABASEREMOVEITEMCOMMAND_H
#define DATABASEREMOVEITEMCOMMAND_H

#include "databasecommand.h"
#include "Commands/itemownercommanddecorator.h"

template<typename T>
class DatabaseRemoveItemCommand : public DatabaseCommand<T>, private ItemOwnerCommandDecorator<T>
{
public:
    DatabaseRemoveItemCommand( Database<T>* database, T* item ) :
        DatabaseCommand<T>( database ),
        ItemOwnerCommandDecorator<T>(item),
        m_index(database->rowOf(item))
    {
        assert( item );
        this->setText( CommandTranslator::tr("Delete Event") );
    }

    void undo()
    {
        this->takeOwnershipOfItem();
        this->model()->insertItem( this->item(), m_index );
    }

    void redo()
    {
        this->releaseOwnershipOfItem();
        this->model()->removeItem( this->item() );

    }

private:
    const int m_index;
};

#endif // DATABASEREMOVEITEMCOMMAND_H
