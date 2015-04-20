#include "chordpatternattachmenteditpatterncommand.h"
#include "global.h"

ChordPatternAttachmentEditPatternCommand::ChordPatternAttachmentEditPatternCommand( ChordPatternAttachment* attachment, const QString & pattern ) :
    AttachmentCommand(attachment),
    m_oldPattern( attachment->chordPattern() ),
    m_newPattern( pattern )
{
}
void ChordPatternAttachmentEditPatternCommand::undo()
{
    qDebug() << "undo " << m_oldPattern;
    attachment<ChordPatternAttachment>()->setPattern( m_oldPattern );
}

void ChordPatternAttachmentEditPatternCommand::redo()
{
    qDebug() << "redo " << m_newPattern;
    attachment<ChordPatternAttachment>()->setPattern( m_newPattern );
}
