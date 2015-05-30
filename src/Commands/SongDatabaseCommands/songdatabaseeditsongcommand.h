#ifndef SONGDATABASEEDITSONGCOMMAND_H
#define SONGDATABASEEDITSONGCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseEditSongCommand : public SongDatabaseCommand
{
public:
    SongDatabaseEditSongCommand(SongDatabase *         songDatabase,
                                const QModelIndex &    index,
                                const QVariant &       newData,
                                const int              role     );

    void redo();
    void undo();

private:
    const QModelIndex     m_index;
    const QVariant        m_newData;
    const QVariant        m_oldData;
    const int             m_role;
};

#endif // SONGDATABASEEDITSONGCOMMAND_H
