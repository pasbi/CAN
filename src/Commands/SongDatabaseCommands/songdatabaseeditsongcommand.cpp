#include "songdatabaseeditsongcommand.h"

SongDatabaseEditSongCommand::SongDatabaseEditSongCommand(SongDatabase *         songDatabase,
                                                         const QModelIndex &    index,
                                                         const QVariant &       newData,
                                                         const int              role) :
    SongDatabaseCommand(songDatabase),
    m_index(index),
    m_newData(newData),
    m_role(role)
{
}

QVariant& SongDatabaseEditSongCommand::getAttribute() const
{
    return database()->songs()[m_index.row()]->attribute(m_index.column());
}

void SongDatabaseEditSongCommand::redo()
{
    QVariant& variant = getAttribute();
    m_oldData = variant;
    variant = m_newData;
    database()->datumChanged(m_index);
}

void SongDatabaseEditSongCommand::undo()
{
    QVariant& variant = getAttribute();
    variant = m_oldData;
    database()->datumChanged(m_index);
}
