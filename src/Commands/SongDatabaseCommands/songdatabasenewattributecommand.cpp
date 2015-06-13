#include "songdatabasenewattributecommand.h"

SongDatabaseNewAttributeCommand::SongDatabaseNewAttributeCommand(SongDatabase *songDatabase, const SongDatabase::AttributeKey &attributeKey) :
    SongDatabaseCommand( songDatabase ),
    m_key(attributeKey)
{
    setText(QString(CommandTranslator::tr("New attribute %1")).arg( m_key ));
}

void SongDatabaseNewAttributeCommand::redo()
{
    database()->appendColumn(m_key);
}

void SongDatabaseNewAttributeCommand::undo()
{
    database()->removeColumns(database()->columnCount() - 1, 1, QModelIndex());
}
