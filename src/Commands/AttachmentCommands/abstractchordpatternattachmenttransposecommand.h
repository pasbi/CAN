#ifndef ABSTRACTCHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
#define ABSTRACTCHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H

#include "Commands/modelcommand.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"

class AbstractChordPatternAttachmentTransposeCommand : public AttachmentCommand<AbstractChordPatternAttachment>
{
public:
    AbstractChordPatternAttachmentTransposeCommand(AbstractChordPatternAttachment *attachment, int transpose);
    void undo();
    void redo();

private:
    const int m_transpose;
};

#endif // ABSTRACTCHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
