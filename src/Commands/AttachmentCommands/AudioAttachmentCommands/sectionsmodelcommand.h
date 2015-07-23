#ifndef SECTIONSMODELCOMMAND_H
#define SECTIONSMODELCOMMAND_H

#include "Commands/command.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"

class SectionsModelCommand : public Command
{
public:
    SectionsModelCommand( SectionsModel* model );
    SectionsModel* model() const { return m_model; }

private:
    SectionsModel* const m_model;
};

#endif // SECTIONSMODELCOMMAND_H
