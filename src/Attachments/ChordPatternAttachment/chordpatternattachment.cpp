#include "chordpatternattachment.h"

#include "../attachment.h"
#include <QApplication>

DEFN_CREATABLE_NAME(ChordPatternAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Chord Attachment"))

const int TAB_WIDTH = 8;

ChordPatternAttachment::ChordPatternAttachment() :
    m_pattern( "" )
{
    setName( tr("Chord Pattern") );
}

void ChordPatternAttachment::copy(Attachment *&copied) const
{
    copied = new ChordPatternAttachment();
    ChordPatternAttachment* cpa = dynamic_cast<ChordPatternAttachment*>(copied);
    cpa->m_pattern = m_pattern;
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

QJsonObject ChordPatternAttachment::toJsonObject() const
{
    QJsonObject object = Attachment::toJsonObject();

    object.insert("pattern", chordPattern());
    object.insert("scrollDownTempo", m_scrollDownTempo);

    return object;
}

bool ChordPatternAttachment::restoreFromJsonObject(const QJsonObject &object)
{
    bool success = true;
    if (checkJsonObject( object, "pattern", QJsonValue::String ))
    {
        m_pattern = object["pattern"].toString();
    }
    else
    {
        m_pattern = "";
        success = false;
    }
    if (checkJsonObject( object, "scrollDownTempo", QJsonValue::Double ))
    {
        m_scrollDownTempo = object["scrollDownTempo"].toDouble();
    }
    else
    {
        m_scrollDownTempo = 0;
        // scrollDownTempo is not as important. keep success.
    }

    return success;
}

void ChordPatternAttachment::setPattern(const QString &pattern)
{
    if (m_pattern != pattern)
    {
        m_pattern = pattern;
        emit changed();
    }
}

void ChordPatternAttachment::process(int transposing)
{
    setPattern( process(m_pattern, transposing) );
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
                QString c = chord.toString(m_minorPolicy, m_enharmonicPolicy);
                text = text.replace(i, token.length(), c);
                additional = c.length() - token.length();
            }
            i += token.length() + 1 + additional;
        }
    }
    return replaceTabs(text);
}

