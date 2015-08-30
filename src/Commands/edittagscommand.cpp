#include "edittagscommand.h"

EditTagsCommand::EditTagsCommand(Taggable *taggable, const QStringList &tags) :
    ModelCommand(taggable),
    m_oldTags( taggable->tags() ),
    m_newTags( tags )
{
    setText( CommandTranslator::tr("Edit Tags") );
}

void EditTagsCommand::undo()
{
    model()->setTags( m_oldTags );
}

void EditTagsCommand::redo()
{
    model()->setTags( m_newTags );
}
