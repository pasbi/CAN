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
    qDebug() << "rename attribute " << index << newData;
}

//QVariant& SongDatabaseEditSongCommand::getAttribute() const
//{
//    return database()->songs()[m_index.row()]->attribute(m_index.column());
//}

void SongDatabaseEditSongCommand::redo()
{
//    QVariant& variant = getAttribute();
//    m_oldData = variant;
//    variant = m_newData;
//    database()->notifyDataChanged(m_index);
    database()->setData( m_index, m_newData, m_role );
}

void SongDatabaseEditSongCommand::undo()
{
//    QVariant& variant = getAttribute();
//    variant = m_oldData;
//    database()->notifyDataChanged(m_index);
    database()->setData( m_index, m_oldData, m_role );
}
