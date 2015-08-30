#include "abstractchordpatternattachmenttransposecommand.h"

AbstractChordPatternAttachmentTransposeCommand::AbstractChordPatternAttachmentTransposeCommand(AbstractChordPatternAttachment* attachment, int transpose) :
    AttachmentCommand(attachment),
    m_transpose( transpose )
{
    setText( QString(CommandTranslator::tr("transpose chord pattern")) );
}

void AbstractChordPatternAttachmentTransposeCommand::undo()
{
    model()->transpose( -m_transpose );
}

void AbstractChordPatternAttachmentTransposeCommand::redo()
{
    model()->transpose( m_transpose );
}
