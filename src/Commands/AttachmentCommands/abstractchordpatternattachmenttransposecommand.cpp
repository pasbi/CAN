#include "abstractchordpatternattachmenttransposecommand.h"

AbstractChordPatternAttachmentTransposeCommand::AbstractChordPatternAttachmentTransposeCommand(AbstractChordPatternAttachment* attachment , int transpose) :
    AttachmentCommand(attachment),
    m_transpose( transpose )
{
    setText( QString(CommandTranslator::tr("Transpose chord pattern %1").arg(m_transpose)) );
}

void AbstractChordPatternAttachmentTransposeCommand::undo()
{
    attachment<AbstractChordPatternAttachment>()->transpose( -m_transpose );
}

void AbstractChordPatternAttachmentTransposeCommand::redo()
{

    attachment<AbstractChordPatternAttachment>()->transpose( m_transpose );
}
