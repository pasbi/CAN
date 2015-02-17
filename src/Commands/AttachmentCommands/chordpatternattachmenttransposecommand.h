#ifndef CHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
#define CHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H

#include "../AttachmentCommands/attachmentcommand.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"

class ChordPatternAttachmentTransposeCommand : public AttachmentCommand
{
public:
    ChordPatternAttachmentTransposeCommand(ChordPatternAttachment *attachment, int transpose);
    void undo();
    void redo();

private:
    const int m_transpose;
};

#endif // CHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
