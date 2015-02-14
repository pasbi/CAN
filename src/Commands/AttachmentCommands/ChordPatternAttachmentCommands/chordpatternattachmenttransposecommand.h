#ifndef CHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
#define CHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H

#include "../ChordPatternAttachmentCommands/chordpatternattachmentcommand.h"

class ChordPatternAttachmentTransposeCommand : public ChordPatternAttachmentCommand
{
public:
    ChordPatternAttachmentTransposeCommand(ChordPatternAttachment *attachment, int transpose);
    void undo();
    void redo();

private:
    const int m_transpose;
};

#endif // CHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
