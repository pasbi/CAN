#ifndef INSERTSECTIONCOMMAND_H
#define INSERTSECTIONCOMMAND_H

#include "Commands/modelcommand.h"
#include "Attachments/AudioAttachment/section.h"

class SectionsModel;
class InsertSectionCommand : public ModelCommand<SectionsModel>
{
public:
    InsertSectionCommand( SectionsModel* model, Section section, int index = -1 );

    void undo();
    void redo();

private:
    Section m_section;
    const int m_index;
};

#endif // INSERTSECTIONCOMMAND_H
