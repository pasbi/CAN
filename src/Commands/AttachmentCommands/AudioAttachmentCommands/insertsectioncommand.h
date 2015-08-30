#ifndef INSERTSECTIONCOMMAND_H
#define INSERTSECTIONCOMMAND_H

#include "Commands/modelcommand.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"

class InsertSectionCommand : public ModelCommand<SectionsModel>
{
public:
    InsertSectionCommand( SectionsModel* model, const Section& section, int index );

    void undo();
    void redo();

private:
    const Section& m_section;
    const int m_index;
};

#endif // INSERTSECTIONCOMMAND_H
