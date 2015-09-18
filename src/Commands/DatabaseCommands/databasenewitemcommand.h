#ifndef DATABASENEWITEMCOMMAND_H
#define DATABASENEWITEMCOMMAND_H

#include "Commands/itemownercommanddecorator.h"
#include "Commands/modelcommand.h"

template<typename T> class Database;
template<typename T>
class DatabaseNewItemCommand : public ModelCommand<Database<T>>, private ItemOwnerCommandDecorator<T>
{
public:
    DatabaseNewItemCommand(Database<T> *database, T* item, int row = -1) :
        ModelCommand<Database<T>>(database),
        ItemOwnerCommandDecorator<T>(item),
        m_row( row )
    {
        this->setText( CommandTranslator::tr("New %1").arg(this->itemTypeName()) );
    }

    ~DatabaseNewItemCommand()
    {
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
