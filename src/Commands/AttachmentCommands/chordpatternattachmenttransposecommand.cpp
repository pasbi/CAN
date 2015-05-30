#include "chordpatternattachmenttransposecommand.h"

ChordPatternAttachmentTransposeCommand::ChordPatternAttachmentTransposeCommand(ChordPatternAttachment* attachment , int transpose) :
    AttachmentCommand(attachment),
    m_transpose( transpose )
{

}

void ChordPatternAttachmentTransposeCommand::undo()
{
    attachment<ChordPatternAttachment>()->process( -m_transpose );
}

void ChordPatternAttachmentTransposeCommand::redo()
{
    attachment<ChordPatternAttachment>()->process( m_transpose );
}
