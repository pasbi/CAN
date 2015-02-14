#ifndef CHORDPATTERNATTACHMENTCOMMAND_H
#define CHORDPATTERNATTACHMENTCOMMAND_H

#include "../attachmentcommand.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"

class ChordPatternAttachmentCommand : public AttachmentCommand
{
public:
    ChordPatternAttachmentCommand(ChordPatternAttachment *attachment);
    ChordPatternAttachment* attachment() const { return AttachmentCommand::attachment<ChordPatternAttachment>(); }
};

#endif // CHORDPATTERNATTACHMENTCOMMAND_H
