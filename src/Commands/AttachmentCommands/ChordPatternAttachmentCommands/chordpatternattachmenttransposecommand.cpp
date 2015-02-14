#include "chordpatternattachmenttransposecommand.h"
#include "Attachments/ChordPatternAttachment/chordpattern.h"

ChordPatternAttachmentTransposeCommand::ChordPatternAttachmentTransposeCommand(ChordPatternAttachment* attachment , int transpose) :
    ChordPatternAttachmentCommand(attachment),
    m_transpose( transpose )
{

}

void ChordPatternAttachmentTransposeCommand::undo()
{
    attachment()->transpose( -m_transpose );
}

void ChordPatternAttachmentTransposeCommand::redo()
{
    attachment()->transpose( m_transpose );
}
