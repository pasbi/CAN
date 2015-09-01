#ifndef DATABASECOMMAND_H
#define DATABASECOMMAND_H

#include "Commands/modelcommand.h"

template<typename T> class Database;
template<typename T>
class DatabaseCommand : public ModelCommand<Database<T>>
{
protected:
    DatabaseCommand(Database<T>* database) :
        ModelCommand<Database<T>>(database)
    {
    }
};

#endif // DATABASECOMMAND_H
