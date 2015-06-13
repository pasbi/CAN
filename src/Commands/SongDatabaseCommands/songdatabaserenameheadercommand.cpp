#include "songdatabaserenameheadercommand.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"

SongDatabaseRenameHeaderCommand::SongDatabaseRenameHeaderCommand(SongDatabaseSortProxy *    songDatabase,
                                                                 const QString &            newName,
                                                                 const int                  section,
                                                                 const Qt::Orientation      orientation) :
    Command(),
    m_songDatabase( songDatabase ),
    m_newName(newName),
    m_section(section),
    m_orientation(orientation)
{
    m_oldName = m_songDatabase->headerData( m_section, m_orientation, Qt::DisplayRole ).toString();

    setText(QString(CommandTranslator::tr("Rename attribute %1 -> %2")).arg( m_oldName ).arg( m_newName ));
}

void SongDatabaseRenameHeaderCommand::redo()
{
    m_songDatabase->setHeaderData( m_section, m_orientation, m_newName, Qt::EditRole );
}

void SongDatabaseRenameHeaderCommand::undo()
{
    m_songDatabase->setHeaderData( m_section, m_orientation, m_oldName, Qt::EditRole );
}
