#include "abstractchordpatternattachmenttransposecommand.h"
#include "Attachments/ChordPatternAttachment/abstractchordpatternattachment.h"

AbstractChordPatternAttachmentTransposeCommand::AbstractChordPatternAttachmentTransposeCommand(AbstractChordPatternAttachment* attachment, int transpose) :
    ModelCommand(attachment),
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
