#ifndef EDITTAGSCOMMAND_H
#define EDITTAGSCOMMAND_H

#include "Commands/modelcommand.h"
#include "taggable.h"

class EditTagsCommand : public ModelCommand<Taggable>
{
public:
    EditTagsCommand(Taggable* taggable, const QStringList& tags);

    void undo();
    void redo();

private:
    const QStringList m_oldTags;
    const QStringList m_newTags;
};

#endif // EDITTAGSCOMMAND_H
