#ifndef SETLISTEDITDATACOMMAND_H
#define SETLISTEDITDATACOMMAND_H

#include "setlistcommand.h"

class SetlistEditDataCommand : public SetlistCommand
{
public:
    SetlistEditDataCommand(Setlist* setlist, const QModelIndex& index, const QVariant& newData, int role);
    void undo();
    void redo();

private:
    QModelIndex m_index;
    QVariant m_newData;
    QVariant m_oldData;
    int m_role;
};

#endif // SETLISTEDITDATACOMMAND_H
