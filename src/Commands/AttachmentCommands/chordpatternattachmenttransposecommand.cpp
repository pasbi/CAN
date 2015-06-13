#include "chordpatternattachmenttransposecommand.h"

ChordPatternAttachmentTransposeCommand::ChordPatternAttachmentTransposeCommand(ChordPatternAttachment* attachment , int transpose) :
    AttachmentCommand(attachment),
    m_transpose( transpose )
{
    setText( QString(CommandTranslator::tr("Transpose chord pattern %1").arg(m_transpose)) );
}

void ChordPatternAttachmentTransposeCommand::undo()
{
    attachment<ChordPatternAttachment>()->process( -m_transpose );
}

void ChordPatternAttachmentTransposeCommand::redo()
{

    attachment<ChordPatternAttachment>()->process( m_transpose );
}
