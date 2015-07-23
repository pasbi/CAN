#ifndef SONGDATABASEREMOVECOLUMNCOMMAND_H
#define SONGDATABASEREMOVECOLUMNCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseRemoveColumnCommand : public SongDatabaseCommand
{
public:
    SongDatabaseRemoveColumnCommand( SongDatabase* songDatabase, const int section );

    void undo();
    void redo();

private:
    const int   m_section;
    QVariantList m_attributes;
    QString m_label;
};

#endif // SONGDATABASEREMOVECOLUMNCOMMAND_H
