#ifndef ABSTRACTCHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
#define ABSTRACTCHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H

#include "Commands/modelcommand.h"

class AbstractChordPatternAttachment;
class AbstractChordPatternAttachmentTransposeCommand : public ModelCommand<AbstractChordPatternAttachment>
{
public:
    AbstractChordPatternAttachmentTransposeCommand(AbstractChordPatternAttachment *attachment, int transpose);
    void undo();
    void redo();

private:
    const int m_transpose;
};

#endif // ABSTRACTCHORDPATTERNATTACHMENTTRANSPOSECOMMAND_H
