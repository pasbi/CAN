#include "chordpatternattachment.h"

#include "../attachment.h"

DEFN_CREATABLE(ChordPatternAttachment, Attachment);

ChordPatternAttachment::ChordPatternAttachment() :
    m_pattern( "" )
{
    setName( tr("Chord Pattern") );
}

QString ChordPatternAttachment::text(Chord::MinorPolicy minorPolicy, Chord::EnharmonicPolicy enharmonicPolicy) const
{
    return m_pattern.toString( minorPolicy, enharmonicPolicy );
}

void ChordPatternAttachment::transpose( int t )
{
    m_pattern.transpose( t );
    emit changed();
}

void ChordPatternAttachment::copy(Attachment *&attachment) const
{
    attachment = new ChordPatternAttachment();
    static_cast<ChordPatternAttachment*>(attachment)->m_pattern = m_pattern;
}

QJsonObject ChordPatternAttachment::toJsonObject() const
{
    QJsonObject object = Attachment::toJsonObject();

    QJsonArray array;
    for (const Line* line : chordPattern()->lines())
    {
        array.append( line->toJsonObject() );
    }

    object.insert("lines", array);

    return object;
}

bool ChordPatternAttachment::restoreFromJsonObject(const QJsonObject &object)
{
    chordPattern()->clear();
    if (checkJsonObject( object, "lines", QJsonValue::Array ))
    {
        for (const QJsonValue & val : object["lines"].toArray())
        {
            if (val.type() == QJsonValue::Object)
            {
                chordPattern()->appendLine( Line::fromJsonObject( val.toObject() ) );
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
