#ifndef CHORDPATTERNATTACHMENTEDITPATTERNCOMMAND_H
#define CHORDPATTERNATTACHMENTEDITPATTERNCOMMAND_H

#include "Commands/modelcommand.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Commands/timerdecorator.h"

class ChordPatternAttachmentEditPatternCommand : public AttachmentCommand<ChordPatternAttachment>, private TimerDecorator
{
public:
    ChordPatternAttachmentEditPatternCommand( ChordPatternAttachment *attachment, const QString &pattern);

    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand *other);

private:
    const QString m_oldPattern;
    QString m_newPattern;
    QString m_mostRecentPattern;
};

#endif // CHORDPATTERNATTACHMENTEDITPATTERNCOMMAND_H
