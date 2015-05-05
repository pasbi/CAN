#ifndef DELETESECTIONCOMMAND_H
#define DELETESECTIONCOMMAND_H

#include "sectionsmodelcommand.h"

class DeleteSectionCommand : public SectionsModelCommand
{
public:
    DeleteSectionCommand( SectionsModel* model, const int row);

    void undo();
    void redo();

private:
    const int m_row;
    Section m_section;
};

#endif // DELETESECTIONCOMMAND_H
