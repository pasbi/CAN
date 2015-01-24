#include "songdatabasenewattributecommand.h"

SongDatabaseNewAttributeCommand::SongDatabaseNewAttributeCommand(SongDatabase *songDatabase) :
    SongDatabaseCommand( songDatabase ),
    m_label(QObject::tr("Unnamed"))
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
