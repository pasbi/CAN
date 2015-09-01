#ifndef DATABASENEWITEMCOMMAND_H
#define DATABASENEWITEMCOMMAND_H

#include "databasecommand.h"
#include "Commands/itemownercommanddecorator.h"

template<typename T>
class DatabaseNewItemCommand : public DatabaseCommand<T>, private ItemOwnerCommandDecorator<T>
{
public:
    DatabaseNewItemCommand(Database<T> *database, T* item, int row = -1) :
        DatabaseCommand<T>(database),
        ItemOwnerCommandDecorator<T>(item),
        m_row( row )
    {
        this->setText( CommandTranslator::tr("Add Song") );
    }

    void redo()
    {
        this->model()->insertItem( this->item(), m_row );
        this->releaseOwnershipOfItem();
    }

    void undo()
    {
        this->model()->removeItem( this->item() );
        this->takeOwnershipOfItem();
    }

private:
    const int m_row;
};

#endif // DATABASENEWITEMCOMMAND_H
