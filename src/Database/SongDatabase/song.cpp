#include "song.h"

Song::Song()
{
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
    if ( index >= m_attributes.size() )
    {
        m_attributes.reserve(index);
        while (index > m_attributes.size() )
        {
            m_attributes.append(QVariant());
        }
        m_attributes.append(data);
    }
    else
    {
        m_attributes[index] = data;
    }
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
