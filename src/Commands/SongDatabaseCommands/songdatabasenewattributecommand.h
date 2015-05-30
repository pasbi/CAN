#ifndef SONGDATABASEADDCOLUMNCOMMAND_H
#define SONGDATABASEADDCOLUMNCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseNewAttributeCommand : public SongDatabaseCommand
{
public:
    SongDatabaseNewAttributeCommand( SongDatabase* songDatabase, const SongDatabase::AttributeKey & attributeKey = QObject::tr("Unnamed") );
    void undo();
    void redo();

private:
    SongDatabase::AttributeKey m_key;
};

#endif // SONGDATABASEADDCOLUMNCOMMAND_H
