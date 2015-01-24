#ifndef SONGDATABASEADDCOLUMNCOMMAND_H
#define SONGDATABASEADDCOLUMNCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseNewAttributeCommand : public SongDatabaseCommand
{
public:
    SongDatabaseNewAttributeCommand( SongDatabase* songDatabase );
    void undo();
    void redo();

private:
    QString m_label;
};

#endif // SONGDATABASEADDCOLUMNCOMMAND_H
