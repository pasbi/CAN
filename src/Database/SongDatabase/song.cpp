#include "song.h"
#include "songdatabase.h"
#include "global.h"

Song::Song(SongDatabase* database) :
    m_songDatabase(database)
{
    int n = m_songDatabase->columnCount();
    m_attributes.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        m_attributes.append(QVariant());
    }
}


void Song::restoreFromJsonObject(const QJsonObject &json)
{
    Taggable::restoreFromJsonObject(json);
}

QJsonObject Song::toJsonObject() const
{
    QJsonObject json = Taggable::toJsonObject();

    return json;
}

QString Song::title() const
{
    return attribute(0).toString();
}

void Song::setTitle(const QString &title)
{
    setAttribute(0, title);
}

void Song::setAttribute(int index, const QVariant &data)
{
    if (index < m_attributes.size())
    {
        m_attributes[index] = data;
    }
    m_songDatabase->notifyDataChanged( this );
}

void Song::insertAttribute(int index, const QVariant &data)
{
    m_attributes.insert(index, data);
}

QVariant Song::attribute(int index) const
{
    if (index < m_attributes.size())
    {
        return m_attributes[index];
    }
    else
    {
        return QVariant();
    }
}

QVariant& Song::attribute(int index)
{
    if (index >= m_attributes.size())
    {
        setAttribute(index, QVariant());
    }
    return m_attributes[index];
}

void Song::removeAttribute(int index)
{
    m_attributes.removeAt(index);
}
