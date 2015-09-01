#ifndef SONGDATABASESETCOLUMNVISIBILITYCOMMAND_H
#define SONGDATABASESETCOLUMNVISIBILITYCOMMAND_H

#include "Commands/modelcommand.h"
#include "Database/SongDatabase/songdatabase.h"

class SongDatabaseSetColumnVisibilityCommand : public ModelCommand<SongDatabase>
{
public:
    SongDatabaseSetColumnVisibilityCommand( SongDatabase* database, int column, bool state );

    void undo();
    void redo();

private:
    const int m_index;
    const bool m_newState;

};

#endif // SONGDATABASESETCOLUMNVISIBILITYCOMMAND_H
