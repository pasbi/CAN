#include "songdatabaserenameheadercommand.h"

SongDatabaseRenameHeaderCommand::SongDatabaseRenameHeaderCommand(SongDatabase *         songDatabase,
                                                                 const QString &        newName,
                                                                 const int              section,
                                                                 const Qt::Orientation  orientation) :
    SongDatabaseCommand(songDatabase),
    m_newName(newName),
    m_section(section),
    m_orientation(orientation)
{
}

void SongDatabaseRenameHeaderCommand::redo()
{
    m_oldName = database()->headerData( m_section, m_orientation, Qt::DisplayRole ).toString();
    database()->setHeaderData( m_section, m_orientation, m_newName, Qt::EditRole );
}

void SongDatabaseRenameHeaderCommand::undo()
{
    database()->setHeaderData( m_section, m_orientation, m_oldName, Qt::EditRole );
}
