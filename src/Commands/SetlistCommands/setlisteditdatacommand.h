#ifndef SETLISTEDITDATACOMMAND_H
#define SETLISTEDITDATACOMMAND_H

#include "Commands/modelcommand.h"

class SetlistEditDataCommand : public ModelCommand<Setlist>
{
public:
    SetlistEditDataCommand(Setlist* setlist, const QModelIndex& index, const QVariant& newData, int role);
    void undo();
    void redo();

private:
    const QModelIndex m_index;
    const QVariant m_newData;
    const QVariant m_oldData;
    const int m_role;
};

#endif // SETLISTEDITDATACOMMAND_H
