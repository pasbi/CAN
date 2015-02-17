#include "chordpatternattachment.h"

#include "../attachment.h"

DEFN_CREATABLE(ChordPatternAttachment, Attachment);

const int TAB_WIDTH = 8;

ChordPatternAttachment::ChordPatternAttachment() :
    m_pattern( "" )
{
    setName( tr("Chord Pattern") );
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
    QString s;
    for (QString line : text.split("\n"))
    {
        int i = -1;
        while ((i = line.indexOf("\t", i+1)) >= 0) {
            line.replace("\t", white(TAB_WIDTH - (i % TAB_WIDTH)));
        }
        s += line + "\n";
    }
    return s.endsWith("\n") ? s.left(s.length() - 1) : s;
}

void ChordPatternAttachment::copy(Attachment *&attachment) const
{
    attachment = new ChordPatternAttachment();
    static_cast<ChordPatternAttachment*>(attachment)->m_pattern = m_pattern;
}

QJsonObject ChordPatternAttachment::toJsonObject() const
{
    QJsonObject object = Attachment::toJsonObject();

    object.insert("pattern", chordPattern());

    return object;
}

bool ChordPatternAttachment::restoreFromJsonObject(const QJsonObject &object)
{
    if (checkJsonObject( object, "pattern", QJsonValue::String ))
    {
        m_pattern = object["pattern"].toString();
        return true;
    }
    else
    {
        m_pattern = "";
        return false;
    }
}

void ChordPatternAttachment::setPattern(const QString &pattern)
{
    if (m_pattern != pattern)
    {
        m_pattern = pattern;
        emit changed();
    }
}

void ChordPatternAttachment::process(int transpose)
{
    QString text = m_pattern;
    int i = 0;
    for (QString line : m_pattern.split("\n")) {
        QStringList chords, tokens;
        bool isChordLine = Chord::parseLine( line, chords, tokens );
        for (QString token : tokens) {
            int additional = 0;
            Chord chord(token);
            if (chord.isValid() && isChordLine) {
                chord.transpose(transpose);
                QString c = chord.toString(m_minorPolicy, m_enharmonicPolicy);
                text = text.replace(i, token.length(), c);
                additional = c.length() - token.length();
            }
            i += token.length() + 1 + additional;
        }
    }
    m_pattern = replaceTabs(text);
    emit changed();
}

