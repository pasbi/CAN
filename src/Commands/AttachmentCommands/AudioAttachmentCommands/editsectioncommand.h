#ifndef EDITSECTIONCOMMAND_H
#define EDITSECTIONCOMMAND_H

#include "Commands/modelcommand.h"
#include <QVariant>
#include <QModelIndex>

class SectionsModel;
class EditSectionCommand : public ModelCommand<SectionsModel>
{
public:
    EditSectionCommand(SectionsModel* model, const QVariant& newValue, const QModelIndex &index, int role );
    void undo();
    void redo();

private:
    const QVariant m_newValue, m_oldValue;
    const QModelIndex m_index;
    const int m_role;
};

#endif // EDITSECTIONCOMMAND_H
