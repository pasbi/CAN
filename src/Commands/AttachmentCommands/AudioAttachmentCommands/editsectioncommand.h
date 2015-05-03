#ifndef EDITSECTIONCOMMAND_H
#define EDITSECTIONCOMMAND_H

#include "../../command.h"
#include "Attachments/AudioAttachment/audioattachment.h"

class EditSectionCommand : public Command
{
public:
    EditSectionCommand( SectionsModel* sectionsModel, const QVariant& newValue, const QModelIndex &index, const int role );
    void undo();
    void redo();

private:
    const QVariant m_newValue, m_oldValue;
    const QModelIndex m_index;
    const int m_role;
    SectionsModel * const m_sectionsModel;
};

#endif // EDITSECTIONCOMMAND_H
