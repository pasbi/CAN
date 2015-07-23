#include "songdatabaseeditsongcommand.h"

SongDatabaseEditSongCommand::SongDatabaseEditSongCommand(SongDatabase *         songDatabase,
                                                         const QModelIndex &    index,
                                                         const QVariant &       newData,
                                                         const int              role) :
    SongDatabaseCommand(songDatabase),
    m_index(index),
    m_newData(newData),
    m_oldData( songDatabase->data( index, role ) ),
    m_role(role)
{
    setText( CommandTranslator::tr("edit song") );
}



void SongDatabaseEditSongCommand::redo()
{
    database()->setData( m_index, m_newData, m_role );
}

void SongDatabaseEditSongCommand::undo()
{
    database()->setData( m_index, m_oldData, m_role );
}
