#include "songdatabasenewattributecommand.h"

SongDatabaseNewAttributeCommand::SongDatabaseNewAttributeCommand(SongDatabase *songDatabase, const SongDatabase::AttributeKey &attributeKey) :
    SongDatabaseCommand( songDatabase ),
    m_key(attributeKey)
{
}

void SongDatabaseNewAttributeCommand::redo()
{
    database()->appendColumn(m_key);
}

void SongDatabaseNewAttributeCommand::undo()
{
    database()->removeColumns(database()->columnCount() - 1, 1, QModelIndex());
}
