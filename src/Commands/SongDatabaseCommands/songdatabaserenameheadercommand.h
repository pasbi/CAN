#ifndef SONGDATABASERENAMEHEADERCOMMAND_H
#define SONGDATABASERENAMEHEADERCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseRenameHeaderCommand : public SongDatabaseCommand
{
public:
    SongDatabaseRenameHeaderCommand(SongDatabase*           songDatabase,
                                    const QString &         newName,
                                    const int               section,
                                    const Qt::Orientation   orientation);

    void redo();
    void undo();

private:
    QString m_newName;
    QString m_oldName;
    int m_section;
    Qt::Orientation m_orientation;
};

#endif // SONGDATABASERENAMEHEADERCOMMAND_H
