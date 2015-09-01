#include "chordpatternattachmenteditpatterncommand.h"
#include "global.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"


ChordPatternAttachmentEditPatternCommand::ChordPatternAttachmentEditPatternCommand( ChordPatternAttachment* attachment, const QString & pattern ) :
    ModelCommand(attachment),
    TimerDecorator(),
    m_oldPattern( attachment->chordPattern() ),
    m_newPattern( pattern ),
    m_mostRecentPattern( m_oldPattern )
{
    setText( CommandTranslator::tr("Edit Chord Pattern") );
}

void ChordPatternAttachmentEditPatternCommand::undo()
{
    model()->setPattern( m_oldPattern );
}

void ChordPatternAttachmentEditPatternCommand::redo()
{
    model()->setPattern( m_newPattern );
}

int ChordPatternAttachmentEditPatternCommand::id() const
{
    return 0;
}

/**
 * @brief oneCharDiff compare two strings, @code oldPattern and @code newPattern
 * @param oldPattern
 * @param newPattern
 * @param diff the char which was added in one of both strings
 * @return whether one of the strings contains exactly one chare more than the other and the strings
 *  equal except of this single char.
 */
bool oneCharDiff(const QString& oldPattern, const QString& newPattern, QChar& diff)
{
    QString longerPattern  = oldPattern.length() > newPattern.length() ? oldPattern : newPattern;
    QString shorterPattern = oldPattern.length() > newPattern.length() ? newPattern : oldPattern;
    int nMax = longerPattern.length();
    int nMin = shorterPattern.length();
    if (nMax - nMin != 1)
    {
        return false;
    }

    int i = 0;
    while (i < nMin && oldPattern[i] == newPattern[i])
    {
        ++i;
    }
    diff = longerPattern[i];
    if (i == nMin)
    {
        return true;    // last char is differnece
    }
    else
    {
        return longerPattern.endsWith(shorterPattern.mid(i));
    }
}

bool ChordPatternAttachmentEditPatternCommand::mergeWith(const QUndoCommand *other)
{
    const ChordPatternAttachmentEditPatternCommand* otherChordPatternAttachmentEditPatternCommand = static_cast<const ChordPatternAttachmentEditPatternCommand*>(other);
    if (otherChordPatternAttachmentEditPatternCommand->m_newPattern == m_newPattern)
    {
        // nothing changed, actually the `other` command should have never been spawned.
        return true;
    }

    QChar secondDiff, firstCharDiff;
    if (oneCharDiff(m_mostRecentPattern, m_newPattern, firstCharDiff)
            && oneCharDiff(otherChordPatternAttachmentEditPatternCommand->m_oldPattern, otherChordPatternAttachmentEditPatternCommand->m_newPattern, secondDiff))
    {
        if (!firstCharDiff.isSpace() && secondDiff.isSpace())
        {
            // we just added or removed a space and in the command before we did not. so do not merge those commands.
            return false;
        }

        if (secondDiff.isSpace())
        {
            // we have two following spaces, merge them!
            m_mostRecentPattern = m_newPattern;
            m_newPattern = otherChordPatternAttachmentEditPatternCommand->m_newPattern;
            return true;
        }
    }

    // two following non-whitespaces, ask timer if merge should happen.
    if (spanExpired())
    {
        return false;
    }
    else
    {
        m_mostRecentPattern = m_newPattern;
        m_newPattern = otherChordPatternAttachmentEditPatternCommand->m_newPattern;
        return true;
    }
}
