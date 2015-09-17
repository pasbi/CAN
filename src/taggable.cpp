#include "taggable.h"
#include "global.h"
#include <QJsonArray>

QMap<QString, int> Taggable::m_allTags;

Taggable::Taggable()
{
}

void removeTag(QMap<QString, int>& counter, const QString & tag )
{
    counter[tag]--;
    if (counter[tag] == 0)
    {
        counter.remove( tag );
    }
}

void addTag(QMap<QString, int>& counter, const QString & tag )
{
    if (!counter.contains( tag ))
    {
        counter[tag]++;
    }
    else
    {
        counter.insert(tag, 1);
    }
}

Taggable::~Taggable()
{
    // remove tags to decrease static counter m_allTags
    for (const QString & tag : m_tags)
    {
        ::removeTag( m_allTags, tag );
    }
}

void Taggable::addTag(const QString & tag)
{
    if (!hasTag( tag ))
    {
        m_tags.append( tag );
    }
    ::addTag( m_allTags, tag );
}

void Taggable::removeTag(const QString & tag)
{
    m_tags.removeOne( tag );
    ::removeTag( m_allTags, tag );
}

bool Taggable::hasTag(const QString & tag) const
{
    return m_tags.contains( tag );
}

void Taggable::setTags(const QStringList &tags)
{
    // to be able to track tags, remove and add each.
    while (!m_tags.isEmpty())
    {
        removeTag( m_tags.takeFirst() );
    }

    for (const QString& tag : tags)
    {
        addTag( tag );
    }
}

void Taggable::serialize(QDataStream &out) const
{
    out << m_tags;
}

void Taggable::deserialize(QDataStream &in)
{
    QStringList tags;
    in >> tags;
    for (const QString& tag : tags)
    {
        addTag(tag);
    }
}
