#ifndef CHORDPATTERNATTACHMENTEDITPATTERNCOMMAND_H
#define CHORDPATTERNATTACHMENTEDITPATTERNCOMMAND_H

#include "attachmentcommand.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"

class ChordPatternAttachmentEditPatternCommand : public AttachmentCommand
{
public:
    ChordPatternAttachmentEditPatternCommand( ChordPatternAttachment *attachment, const QString &pattern);

    void undo();
    void redo();

private:
    const QString m_oldPattern;
    const QString m_newPattern;
};

#endif // CHORDPATTERNATTACHMENTEDITPATTERNCOMMAND_H
