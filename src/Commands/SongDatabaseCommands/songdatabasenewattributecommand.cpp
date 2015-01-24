#include "songdatabasenewattributecommand.h"

SongDatabaseNewAttributeCommand::SongDatabaseNewAttributeCommand(SongDatabase *songDatabase, const QString & label) :
    SongDatabaseCommand( songDatabase ),
    m_label(label)
{

}

void SongDatabaseNewAttributeCommand::redo()
{
    database()->appendColumn(m_label);
}

void SongDatabaseNewAttributeCommand::undo()
{
    database()->removeColumns(database()->columnCount() - 1, 1, QModelIndex());
}
