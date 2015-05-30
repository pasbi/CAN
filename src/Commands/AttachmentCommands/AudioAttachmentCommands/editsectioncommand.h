#ifndef EDITSECTIONCOMMAND_H
#define EDITSECTIONCOMMAND_H

#include "sectionsmodelcommand.h"
#include "Attachments/AudioAttachment/audioattachment.h"

class EditSectionCommand : public SectionsModelCommand
{
public:
    EditSectionCommand( SectionsModel* model, const QVariant& newValue, const QModelIndex &index, const int role );
    void undo();
    void redo();

private:
    const QVariant m_newValue, m_oldValue;
    const QModelIndex m_index;
    const int m_role;
};

#endif // EDITSECTIONCOMMAND_H
