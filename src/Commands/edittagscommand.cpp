#include "edittagscommand.h"
#include "taggable.h"

EditTagsCommand::EditTagsCommand(Taggable *taggable, const QStringList &tags) :
    ModelCommand(taggable),
    m_oldTags( taggable->tags() ),
    m_newTags( tags )
{
}

void EditTagsCommand::undo()
{
    model()->setTags( m_oldTags );
}

void EditTagsCommand::redo()
{
    model()->setTags( m_newTags );
}
