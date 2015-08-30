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
    setText( CommandTranslator::tr("Edit Song") );
}

void SongDatabaseEditSongCommand::redo()
{
    model()->setData( m_index, m_newData, m_role );
}

void SongDatabaseEditSongCommand::undo()
{
    model()->setData( m_index, m_oldData, m_role );
}
