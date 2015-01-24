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
    QModelIndex     m_index;
    QVariant        m_newData;
    QVariant        m_oldData;
    int             m_role;

    QVariant& getAttribute() const;




};

#endif // SONGDATABASEEDITSONGCOMMAND_H
