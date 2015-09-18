#include "abstractchordpatternattachmenttransposecommand.h"
#include "Attachments/ChordPatternAttachment/abstractchordpatternattachment.h"

AbstractChordPatternAttachmentTransposeCommand::AbstractChordPatternAttachmentTransposeCommand(AbstractChordPatternAttachment* attachment, int transpose) :
    ModelCommand(attachment),
    m_transpose( transpose )
{
}

void AbstractChordPatternAttachmentTransposeCommand::undo()
{
    model()->transpose( -m_transpose );
}

void AbstractChordPatternAttachmentTransposeCommand::redo()
{
    model()->transpose( m_transpose );
}
