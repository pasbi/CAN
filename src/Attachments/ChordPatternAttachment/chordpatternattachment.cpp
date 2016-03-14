#include "chordpatternattachment.h"

#include "../attachment.h"
#include "Project/project.h"
#include "chord.h"

DEFN_CREATABLE_NAME(ChordPatternAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Chord Attachment"))


const int TAB_WIDTH = 8;

ChordPatternAttachment::ChordPatternAttachment() :
    AbstractChordPatternAttachment()
{
    addAttributeKey("chordPattern");
    addAttributeKey("scrollDownTempo");

    setName( tr("Chord Pattern") );
    setAttribute("chordPattern", "");
    setAttribute("scrollDownTempo", 0);
}

QString white(int n)
{
    QString s;
    for (int i = 0; i < n; i++) {
        s += " ";
    }
    return s;
}

QString replaceTabs(QString text)
{
    QStringList lines = text.split("\n");
    for (QString& line : lines)
    {
        int i = -1;
        while ( (i = line.indexOf("\t", i+1)) >= 0 )
        {
            line.replace(i, 1, white( TAB_WIDTH - (i % TAB_WIDTH) ));
        }
    }
    return lines.join("\n");
}

void ChordPatternAttachment::setPattern(const QString &pattern)
{
    if (chordPattern() != pattern)
    {
        setAttribute("chordPattern", pattern);
        emit changed();
    }
}

void ChordPatternAttachment::transpose(int transposing)
{
    setPattern( process(chordPattern(), transposing) );
    emit transposed(transposing);
}

QString ChordPatternAttachment::process(QString text, int transpose)
{
    int i = 0;

    for (QString line : text.split("\n")) {
        QStringList chords, tokens;
        bool isChordLine = Chord::parseLine( line, chords, tokens );
        for (QString token : tokens)
        {
            int additional = 0;
            Chord chord(token);
            if (chord.isValid() && isChordLine)
            {
                chord.transpose(transpose);
                QString c = chord.toString();
                text = text.replace(i, token.length(), c);
                additional = c.length() - token.length();
            }
            i += token.length() + 1 + additional;
        }
    }
    return replaceTabs(text);
}

double ChordPatternAttachment::scrollDownTempo() const
{
    return attribute("scrollDownTempo").toDouble();
}

QString ChordPatternAttachment::chordPattern() const
{
    return attribute("chordPattern").toString();
}

void ChordPatternAttachment::setScrollDownTempo( double tempo )
{
    if (scrollDownTempo() != tempo)
    {
        setAttribute("scrollDownTempo", tempo);
        pushCommand( nullptr ); // push empty command to indicate project changed.
    }
}
