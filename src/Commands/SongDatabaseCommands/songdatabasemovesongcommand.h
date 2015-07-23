#ifndef SONGDATABASEMOVESONGCOMMAND_H
#define SONGDATABASEMOVESONGCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseMoveSongCommand : public SongDatabaseCommand
{
public:
    SongDatabaseMoveSongCommand(SongDatabase* database, int sourceRow, int destRow );
    void undo();
    void redo();

private:

    const int m_sourceRow;
    const int m_targetRow;

};

#endif // SONGDATABASEMOVESONGCOMMAND_H
