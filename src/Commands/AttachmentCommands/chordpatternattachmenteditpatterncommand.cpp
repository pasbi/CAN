#include "chordpatternattachmenteditpatterncommand.h"
#include "global.h"

void shorten( QString& text, int max )
{
    QString dots = "";

    if (!text.isEmpty())
    {
        QStringList lines = text.split("\n");
        if (lines.length() > 1)
        {
            text = lines.first();
            dots = " ...";
        }
    }

    if (text.length() > max)
    {
        dots = " ...";
        text = text.left(max - dots.length());
    }

    text += dots;
}

void findDifference( const QString& a, const QString& b, QString& diffA, QString& diffB )
{
    int n = qMin( a.length(), b.length() );
    int k = 0;
    for (k = 0; k < n; ++k)
    {
        if (a[k] != b[k])
        {
            break;
        }
    }

    diffA = a.mid(k);
    diffB = b.mid(k);
}



ChordPatternAttachmentEditPatternCommand::ChordPatternAttachmentEditPatternCommand( ChordPatternAttachment* attachment, const QString & pattern ) :
    AttachmentCommand(attachment),
    m_oldPattern( attachment->chordPattern() ),
    m_newPattern( pattern )
{
    QString oldPattern, newPattern;
    findDifference( m_oldPattern, m_newPattern, oldPattern, newPattern );
    shorten( oldPattern, 30 );
    shorten( newPattern, 30 );
    setText( QString(CommandTranslator::tr("Edit chord pattern %1 -> %2").arg(oldPattern, newPattern)) );
}

void ChordPatternAttachmentEditPatternCommand::undo()
{
    attachment<ChordPatternAttachment>()->setPattern( m_oldPattern );
}

void ChordPatternAttachmentEditPatternCommand::redo()
{
    attachment<ChordPatternAttachment>()->setPattern( m_newPattern );
}
