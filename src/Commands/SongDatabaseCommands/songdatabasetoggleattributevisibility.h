#ifndef SONGDATABASETOGGLEATTRIBUTEVISIBILITY_H
#define SONGDATABASETOGGLEATTRIBUTEVISIBILITY_H

#include "songdatabasecommand.h"

//TODO rename to SongDatabaseSetColumnVisibilityCommand
class SongDatabaseToggleAttributeVisibility : public SongDatabaseCommand
{
public:
    SongDatabaseToggleAttributeVisibility( SongDatabase* database, int column, bool state );

    void undo();
    void redo();

private:
    const int m_index;
    const bool m_newState;

};

#endif // SONGDATABASETOGGLEATTRIBUTEVISIBILITY_H
