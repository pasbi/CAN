#include "edittagscommand.h"

EditTagsCommand::EditTagsCommand(Taggable *taggable, const QStringList &tags) :
    m_taggable( taggable ),
    m_oldTags( taggable->tags() ),
    m_newTags( tags )
{
    setText( CommandTranslator::tr("edit tags") );
}

void EditTagsCommand::undo()
{
    m_taggable->setTags( m_oldTags );
}

void EditTagsCommand::redo()
{
    m_taggable->setTags( m_newTags );
}
