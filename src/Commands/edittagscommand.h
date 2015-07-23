#ifndef EDITTAGSCOMMAND_H
#define EDITTAGSCOMMAND_H

#include "command.h"
#include "taggable.h"

class EditTagsCommand : public Command
{
public:
    EditTagsCommand(Taggable* taggable, const QStringList& tags);

    void undo();
    void redo();

private:
    Taggable* const m_taggable;
    const QStringList m_oldTags;
    const QStringList m_newTags;
};

#endif // EDITTAGSCOMMAND_H
