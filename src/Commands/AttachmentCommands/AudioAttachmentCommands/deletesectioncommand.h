#ifndef DELETESECTIONCOMMAND_H
#define DELETESECTIONCOMMAND_H

#include "Commands/modelcommand.h"
#include "Attachments/AudioAttachment/section.h"

class SectionsModel;
class DeleteSectionCommand : public ModelCommand<SectionsModel>
{
public:
    DeleteSectionCommand(SectionsModel* model, int row);

    void undo();
    void redo();

private:
    const int m_row;
    Section m_section;
};

#endif // DELETESECTIONCOMMAND_H
